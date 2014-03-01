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
	char *axe;
	static Vector3d vertix;

	ply_get_argument_user_data(argument, (void **)(&axe), NULL);

	switch (*axe) {
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

std::vector <Face> *faces;

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
		faces->push_back(face);
		face.vertices.clear();
	}

	return 1;
}

static int parse_object(char *fname, int type)
{
	p_ply ply;
	int ret = -1;

	vertices.clear();

	ply = ply_open(fname, NULL, 0, NULL);
	if (!ply)
		err(1, "Can't open ply file");

	if (!ply_read_header(ply))
		err(1, "Can't read ply header");

	ply_set_read_cb(ply, "vertex", "x", add_vertix, (void *)"x", 0);
	ply_set_read_cb(ply, "vertex", "y", add_vertix, (void *)"y", 0);
	ply_set_read_cb(ply, "vertex", "z", add_vertix, (void *)"z", 0);

	switch (type) {
	case SOURCE:
        break;
	case SAMPLE:
		samples.push_back(Sample());
		samples.back().k = 1.;
		faces = &(samples.back().faces);
		break;
	case DETECTOR:
		detectors.push_back(Detector());
		faces = &(detectors.back().faces);
		break;
	default:
		err(1, "Unknown type");
	}

    if (type != SOURCE)
        ply_set_read_cb(ply, "face", "vertex_indices", add_face, NULL, 0);

	if (!ply_read(ply))
		err(1, "can't read ply");

    if (type == SOURCE) {
        sources.push_back(Source());
		sources.back().r = vertices.back();
		puts("ololo");
        sources.back().intencity = 1.;
    }

	ret = 0;

	if (ply)
		ply_close(ply);
	return ret;
}

int parse_input_file(char *fname)
{
	FILE *input;
	char buf[MAX_STR];
	int type = -1;
	int ret = -1;

	input = fopen(fname, "r");
	if (!input) {
		perror("Can't open input file: %s");
		goto out;
	}

	while(fscanf(input, "%s\n", buf) != EOF) {
		puts(buf);
		if (!strncmp(buf, SOURCES_STR, strlen(SOURCES_STR)))
			type = SOURCE;
		else if (!strncmp(buf, SAMPLES_STR, strlen(SAMPLES_STR)))
			type = SAMPLE;
		else if (!strncmp(buf, DETECTORS_STR, strlen(DETECTORS_STR)))
			type = DETECTOR;
		else {
			if (type == -1) {
				perror("No section header is set.");
				goto out;
			}

			if (parse_object(buf, type))
				goto out;
		}
	}

	ret = 0;
out:
	if (input)
		fclose(input);

	return ret;

}
