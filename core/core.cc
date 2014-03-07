#include <string.h>
#include <stdio.h>
#include <err.h>

#include "core.hh"
#include "rply.h"

#define MAX_STR 256
#define SOURCES_STR "[sources]"
#define SAMPLES_STR "[samples]"
#define DETECTORS_STR "[detectors]"

#define SOURCE 0
#define SAMPLE 1
#define DETECTOR 2

#define MAX_SOURCES 1
#define MAX_SAMPLES 1
#define MAX_DETECTORS 1

static std::vector <Vector3d> vertices;

/* ZERO IS BAD AND NON-ZERO IS GOOD! */
static int add_vertix(p_ply_argument argument)
{
	char *axis;
	static Vector3d vertix;

	ply_get_argument_user_data(argument, (void **)(&axis), NULL);

	switch (*axis) {
	case 'x':
		vertix[0] = ply_get_argument_value(argument);
		break;
	case 'y':
		vertix[1] = ply_get_argument_value(argument);
		break;
	case 'z':
		vertix[2] = ply_get_argument_value(argument);
		vertices.push_back(vertix);
		break;
	default:
		perror("Wrong user data");
		return 0;
	}

	return 1;
}

std::vector <Face> faces;

static int add_face(p_ply_argument argument)
{
	static Face face;
	long int length, value_index;

	ply_get_argument_user_data(argument, NULL, NULL);

	ply_get_argument_property(argument, NULL, &length, &value_index);

    if (value_index < 0)
        return 1;

	face.vertices.push_back(vertices[ply_get_argument_value(argument)]);

	if (value_index+1 == length) {
		faces.push_back(face);
		face.vertices.clear();
	}

	return 1;
}

void Core::add_source_from_file(char *fname)
{
    p_ply ply;
    size_t nvertices;

    ply = ply_open(fname, NULL, 0, NULL);
	if (!ply)
		err(1, "Can't open ply file");

	if (!ply_read_header(ply))
		err(1, "Can't read ply header");

    nvertices = ply_set_read_cb(ply, "vertex", "x", add_vertix, (void *)"x", 0);
    if (nvertices != 1)
        err(1, "too many vertices");

	ply_set_read_cb(ply, "vertex", "y", add_vertix, (void *)"y", 0);
	ply_set_read_cb(ply, "vertex", "z", add_vertix, (void *)"z", 0);

	if (!ply_read(ply))
		err(1, "can't read ply");

    sources.push_back(Source());
    sources.back().r = vertices.back();
    sources.back().intencity = 1.;

    /* Cleanup */
    vertices.clear();

	if (ply)
		ply_close(ply);
}

void Core::add_sample_from_file(char *fname)
{
    p_ply ply;

    ply = ply_open(fname, NULL, 0, NULL);
	if (!ply)
		err(1, "Can't open ply file");

	if (!ply_read_header(ply))
		err(1, "Can't read ply header");

    ply_set_read_cb(ply, "vertex", "x", add_vertix, (void *)"x", 0);
	ply_set_read_cb(ply, "vertex", "y", add_vertix, (void *)"y", 0);
	ply_set_read_cb(ply, "vertex", "z", add_vertix, (void *)"z", 0);

    ply_set_read_cb(ply, "face", "vertex_indices", add_face, NULL, 0);

	if (!ply_read(ply))
		err(1, "can't read ply");

    samples.push_back(Sample());
    samples.back().k = 1.;
    samples.back().faces = faces;

    /* Cleanup for */
    vertices.clear();

	if (ply)
		ply_close(ply);
}

void Core::add_detector_from_file(char *fname)
{
    p_ply ply;

    ply = ply_open(fname, NULL, 0, NULL);
	if (!ply)
		err(1, "Can't open ply file");

	if (!ply_read_header(ply))
		err(1, "Can't read ply header");

    ply_set_read_cb(ply, "vertex", "x", add_vertix, (void *)"x", 0);
	ply_set_read_cb(ply, "vertex", "y", add_vertix, (void *)"y", 0);
	ply_set_read_cb(ply, "vertex", "z", add_vertix, (void *)"z", 0);

    ply_set_read_cb(ply, "face", "vertex_indices", add_face, NULL, 0);

	if (!ply_read(ply))
		err(1, "can't read ply");

    detectors.push_back(Detector());
    detectors.back().faces = faces;

    /* Cleanup for */
    vertices.clear();

	if (ply)
		ply_close(ply);
}

void Core::add_objects_from_file(char *fname)
{
	FILE *input;
	char buf[MAX_STR];
	int type = -1;

	input = fopen(fname, "r");
	if (!input)
		err(1, "Can't open input file");

	while(fscanf(input, "%s\n", buf) != EOF) {
		/*
         * Define type of objects
         */
		if (!strncmp(buf, SOURCES_STR, strlen(SOURCES_STR))) {
			type = SOURCE;
            continue;
		} else if (!strncmp(buf, SAMPLES_STR, strlen(SAMPLES_STR))) {
			type = SAMPLE;
			continue;
		} else if (!strncmp(buf, DETECTORS_STR, strlen(DETECTORS_STR))) {
			type = DETECTOR;
			continue;
        }

        /*
         * Call function to add object from .ply file
         */
        switch (type) {
        case SOURCE:
            add_source_from_file(buf);
            break;
        case SAMPLE:
            add_sample_from_file(buf);
            break;
        case DETECTOR:
            add_detector_from_file(buf);
            break;
        default:
				err(1, "No section header is set.");
        }
	}

	if (input)
		fclose(input);
}

void Core::run(void)
{
    return;
}

Vector3d Core::check_intersection(Face face, Vector3d ray_start, Vector3d ray_end)
{
    /*
     * plane: R = R0 + b*u + c*v
     * line: r = r0 + a*t
     * System to solve: r0-R0 = (b,c,-a)*(u,v,t)
     */
    Vector3d a, b, c, p, x, d;
    Matrix3d M;
    double relative_error;
    double phi = 0.0;
    unsigned int i;

    a = ray_end - ray_start;

    b = face.vertices[1] - face.vertices[0];
    c = face.vertices[2] - face.vertices[0];

    M.col(0) = b;
    M.col(1) = c;
    M.col(2) = -a;

    d = ray_start - face.vertices[0];

    p = M.fullPivLu().solve(d);

    relative_error = (M*x - d).norm() / d.norm();
    if (relative_error > 0.01)
        throw false;

    x = ray_start + a * p[2];

    /*
     * Check, if x is in the polygon
     */

    for (i=0; i < face.vertices.size()-1; i++) {
        Vector3d q, w;
        q = x - face.vertices[i];
        w = x - face.vertices[i+1];
        phi += acos(abs(q.dot(w))/(q.norm()*w.norm()));
    }

    relative_error = abs(2.*3.14 - phi)/(2.*3.14);
    if (relative_error > 0.01)
        throw false;

    return x;
}
