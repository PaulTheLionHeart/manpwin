#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "paulslib.h"

/*
	Create rounded cube with texture coordinates in obj format
	ps: Designed to illustrate/verify the apporach, not good coding practices.
*/

double mypower(double,double);

int N = 64; // Mesh resolution
double rpower = 1; // Power that controls corner strength

typedef struct {
	double x,y,z;
	double u,v;
} MESHVERTEX;

int main(int argc,char **argv)
{
	int i,j,index;
	int i1,i2,i3,i4;
	double theta,phi;
	MESHVERTEX *p = NULL,n;
	double len;
	FILE *fptr;

	if (argc < 3) {
		fprintf(stderr,"Usage: %s nres rpower\n",argv[0]);
		exit(-1);
	}
	N = atoi(argv[1]);
	rpower = atof(argv[2]);

	// Create vertices
	if ((p = malloc((N+1)*(N/2+1)*sizeof(MESHVERTEX))) == NULL) {
		fprintf(stderr,"malloc failed\n");
		exit(-1);
	}

	// Pole is along the z axis
	for (j=0;j<=N/2;j++) {
		for (i=0;i<=N;i++) {
			index = j * (N+1) + i;
			theta = i * 2 * M_PI / N;
			phi = -0.5*M_PI + M_PI * j / (N/2.0);

			// Unit sphere, power determines roundness
			p[index].x = mypower(cos(phi),rpower) * mypower(cos(theta),rpower);
			p[index].y = mypower(cos(phi),rpower) * mypower(sin(theta),rpower);
			p[index].z = mypower(sin(phi),rpower);

         // Texture coordinates
         // Project texture coordinates from a surrounding sphere
         p[index].u = atan2(p[index].y,p[index].x) / (2*M_PI);
         if (p[index].u < 0)
            p[index].u = 1 + p[index].u;
         p[index].v = 0.5 + atan2(p[index].z,sqrt(p[index].x*p[index].x+p[index].y*p[index].y)) / M_PI;

			// Seams
			if (j == 0) {
				p[index].x = 0;
				p[index].y = 0;
				p[index].z = -1;
			}
         if (j == N/2) {
            p[index].x = 0;
            p[index].y = 0;
            p[index].z = 1;
         }
			if (i == N) {
				p[index].x = p[j*(N+1)+i-N].x;
            p[index].y = p[j*(N+1)+i-N].y;
			}

		}
	}

	// Save as textured obj file
	fptr = fopen("roundcube.obj","w");
	fprintf(fptr,"mtllib roundcube.mtl\n");
	// Vertices
   for (j=0;j<=N/2;j++) {
      for (i=0;i<=N;i++) {
         index = j * (N+1) + i;
			fprintf(fptr,"v %lf %lf %lf\n",p[index].x,p[index].y,p[index].z);
      }
   }
	// Normals (same as vertices in this case but normalised)
   for (j=0;j<=N/2;j++) {
      for (i=0;i<=N;i++) {
         index = j * (N+1) + i;
         n = p[index];
         len = sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
         fprintf(fptr,"vn %lf %lf %lf\n",n.x/len,n.y/len,n.z/len);
      }
   }
	// Texture coordinates
   for (j=0;j<=N/2;j++) {
		for (i=0;i<=N;i++) {
			index = j * (N+1) + i;
			fprintf(fptr,"vt %lf %lf\n",p[index].u,p[index].v);
		}
	}
	// Faces, each a triangle
	// Vertex count starts at 1 not 0
	fprintf(fptr,"usemtl material0\n");
	for (j=0;j<N/2;j++) {
		for (i=0;i<N;i++) {
			i1 =  j    * (N+1) + i       + 1;
			i2 =  j    * (N+1) + (i + 1) + 1;
			i3 = (j+1) * (N+1) + (i + 1) + 1;
			i4 = (j+1) * (N+1) + i       + 1;
			fprintf(fptr,"f %d/%d/%d %d/%d/%d %d/%d/%d\n",i1,i1,i1,i2,i2,i2,i3,i3,i3);
			fprintf(fptr,"f %d/%d/%d %d/%d/%d %d/%d/%d\n",i1,i1,i1,i3,i3,i3,i4,i4,i4);
		}
	}
	fclose(fptr);

	// Create placeholder material file
	fptr = fopen("roundcube.mtl","w");
	fprintf(fptr,"# Create as many materials as desired\n");
	fprintf(fptr,"# Each is referenced by name before the faces it applies to in the obj file\n");
	fprintf(fptr,"\n");
	for (i=0;i<2;i++) {
		fprintf(fptr,"newmtl material%d\n",i);
		fprintf(fptr,"Ka 1.000000 1.000000 1.000000\n");
		fprintf(fptr,"Kd 1.000000 1.000000 1.000000\n");
		fprintf(fptr,"Ks 0.000000 0.000000 0.000000\n");
		fprintf(fptr,"Tr 1.000000\n");
		fprintf(fptr,"illum 1\n");
		fprintf(fptr,"Ns 0.000000\n");
		fprintf(fptr,"map_Kd roundcube%d.jpg\n",i);
		fprintf(fptr,"\n");
	}
	fclose(fptr);

	exit(0);
}

double mypower(double v,double n)
{
	if (v >= 0)
		return(pow(v,n));
	else
		return(-pow(-v,n));
}

