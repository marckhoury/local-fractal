#include "colormesh.h"

ColorMesh::ColorMesh(Mesh& mesh, FractalGrid& fg, float low, float high) :
    mesh(mesh),
    fg(fg),
    low(low),
    high(high)
{
	use_4d = false;
	colors.resize(mesh.vertex_count());
}

ColorMesh::ColorMesh(Mesh& mesh, FractalGrid4D& fg4d, float low, float high) :
    mesh(mesh),
    fg4d(fg4d),
    low(low),
    high(high)
{
	use_4d = true;
	colors.resize(mesh.vertex_count());
}

void ColorMesh::apply_color(Texture &tex)
{
	for (int i = 0; i < mesh.vertex_count(); i++) {
		Vertex v = mesh.get_vertex(i);
		float coord,loc_dim;
		float f1,f2,t;
		if (use_4d) {
			v.x = v.x / fg4d.get_spacing(0);
			v.y = v.y / fg4d.get_spacing(1);
			v.z = v.z / fg4d.get_spacing(2);
		} else {
			v.x = v.x / fg.get_spacing(0);
			v.y = v.y / fg.get_spacing(1);
			v.z = v.z / fg.get_spacing(2);
		}
		choose_edge(v, f1, f2, t);
		loc_dim = lerp(f1, f2, t);
		if (loc_dim <= low) {
			coord = 0;
        } else if (loc_dim >= high) {
			coord = 1;
		} else {
			coord = (loc_dim - low) / (high-low);
        }		
        colors[i] = tex[coord];
	}
	write_off();
}

float ColorMesh::lerp(float l, float r, float t)
{
	return (1 - t)*l + t*r;
}

void ColorMesh::choose_edge(Vertex& v, float& l, float& r, float& t)
{
	if (use_4d) {
		l = fg4d[fg4d.index(v.x, v.y, v.z)];
    } else {
		l = fg[fg.index(v.x, v.y, v.z)];
    }	

    if (v.x - static_cast<int>(v.x) >= EPSILON) {
		if (use_4d) {
			r = fg4d[fg4d.index(v.x + 1, v.y, v.z)];
		} else {
			r = fg[fg.index(v.x + 1, v.y, v.z)];
		}
        t = v.x - static_cast<int>(v.x);
	} else if (v.y - static_cast<int>(v.y) >= EPSILON) {
		if (use_4d) {
			r = fg4d[fg4d.index(v.x, v.y + 1, v.z)];
		} else {
			r = fg[fg.index(v.x, v.y + 1, v.z)];
		}
        t = v.y - static_cast<int>(v.y);
	} else {
		if (use_4d) {
			r = fg4d[fg4d.index(v.x, v.y, v.z + 1)];
		} else {
			r = fg[fg.index(v.x, v.y, v.z + 1)];
		}
        t = v.z - static_cast<int>(v.z);
	}
}

void ColorMesh::write_off()
{
	string out_file = mesh.to_string() + ".l=" + str(low) + ".h=" + str(high) +".fractal_color.off";
	ofstream out(out_file.c_str());
	out << "CCOFF" << endl;
	out << mesh.vertex_count() << " " << mesh.triangle_count() <<  " 0" << endl;
	for (int i = 0; i < mesh.vertex_count(); i++) {
		Vertex v = mesh.get_vertex(i);
		Color c = colors[i];
		out << v.x << " " << v.y << " " << v.z << " ";
		out << c.r << " " << c.g << " " << c.b << " " << c.a << " ";		
		out << c.r << " " << c.g << " " << c.b << " " << c.a << endl;
	}
	for (int i = 0; i < mesh.triangle_count(); i++) {
		Triangle t = mesh.get_triangle(i);
		out << "3 " << t.v[0] << " " << t.v[1] << " " << t.v[2] << endl;
	}
	cout << "Wrote mesh " << out_file << endl;
	out.close();
}
