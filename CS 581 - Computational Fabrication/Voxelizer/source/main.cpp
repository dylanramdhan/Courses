//Computational Fabrication Assignment #1
// By David Levin 2014
#include <iostream>
#include <vector>
#include "../include/CompFab.h"
#include "../include/Mesh.h"

/* compiling: 
*        --> make
*        --> ./voxelizer ../data/sphere/Sphere.obj ../data/sphere/[outputname].obj
*/


//Ray-Triangle Intersection
//Returns 1 if triangle and ray intersect, 0 otherwise
int rayTriangleIntersection(CompFab::Ray &ray, CompFab::Triangle &triangle)
{
    /********* ASSIGNMENT *********/
    /* Ray-Triangle intersection test: Return 1 if ray intersects triangle, 
     * 0 otherwise */
    
    CompFab::Vec3 x = triangle.m_v1;
    CompFab::Vec3 y = triangle.m_v2;
    CompFab::Vec3 z = triangle.m_v3; // v3 = C

    CompFab::Vec3 origin = ray.m_origin; // ray = P
    CompFab::Vec3 dir = ray.m_direction; // dir = d
    

    CompFab::Vec3 y_x = CompFab::operator-(y, x);
    CompFab::Vec3 z_x = CompFab::operator-(z, x); // followed lec slides
    
    CompFab::Vec3 z_y = CompFab::operator-(z, y); // Created for Dot Product
    CompFab::Vec3 x_z = CompFab::operator-(x, z); // Created for Dot Product
    
    CompFab::Vec3 norm = CompFab::operator%(y_x, z_x); // operator
    
    // Normalizing the Vector
    //CompFab::Vec3Struct norm = CompFab::operator%((y - x), (z - x)), CompFab::operator%((y - x), (z - x));
    
    //(CompFab::operator%(y_x, z_x));
    norm.normalize(); // nornmalize is already defined in 'CompFab.h'
    
    if (CompFab::operator*(dir, norm) == 0)
    {
        return 0; // false
    }

    float t = CompFab::operator*((CompFab::operator-(z, origin)), norm)/ CompFab::operator*(dir, norm);

    if (t>0) // if t is greater than 0, then it intersects
    {
        CompFab::Vec3 td = CompFab::Vec3(t*dir.m_x, t*dir.m_y, t*dir.m_z);
        CompFab::Vec3 q = CompFab::operator+(origin, td);
    
        CompFab::Vec3 q_x = CompFab::operator-(q, x);
        CompFab::Vec3 q_y = CompFab::operator-(q, y);
        CompFab::Vec3 q_z = CompFab::operator-(q, z);

        // Calculating the Result
        float result1 = CompFab::operator*(CompFab::operator%(y_x, q_x), norm);
        float result2 = CompFab::operator*(CompFab::operator%(z_y, q_y), norm);
        float result3 = CompFab::operator*(CompFab::operator%(x_z, q_z), norm);
        
            // Showing Results, Outputting True/False
            if (result1 > 0 && result2 > 0 && result3 > 0)
            {
                return 1; // true
            }
            else
            {
                return 0; // false
            }
    }
    
    return 0;

}

//Triangle list (global)
typedef std::vector<CompFab::Triangle> TriangleList;

TriangleList g_triangleList;
CompFab::VoxelGrid *g_voxelGrid;

//Number of intersections with surface made by a ray originating at voxel and cast in direction.
int numSurfaceIntersections(CompFab::Vec3 &voxelPos, CompFab::Vec3 &dir)
{
    unsigned int numHits = 0;
    
    /********* ASSIGNMENT *********/
    /* Check and return the number of times a ray cast in direction dir, 
     * from voxel center voxelPos intersects the surface */ 
        for (int i = 0; i < g_triangleList.size(); i++)
        {
            g_triangleList[i]; // taking one triangle
            
            CompFab::Ray ray = CompFab::Ray(voxelPos, dir);
            CompFab::Triangle tri = g_triangleList[i];
            
            // 'result' stores return value
            int result = rayTriangleIntersection(ray, tri);

            if (result == 1)
            {
                numHits++;
            }
        }
    





    return numHits;
}

bool loadMesh(char *filename, unsigned int dim)
{
    g_triangleList.clear();
    
    Mesh *tempMesh = new Mesh(filename, true);
    
    CompFab::Vec3 v1, v2, v3;

    //copy triangles to global list
    for(unsigned int tri =0; tri<tempMesh->t.size(); ++tri)
    {
        v1 = tempMesh->v[tempMesh->t[tri][0]];
        v2 = tempMesh->v[tempMesh->t[tri][1]];
        v3 = tempMesh->v[tempMesh->t[tri][2]];
        g_triangleList.push_back(CompFab::Triangle(v1,v2,v3));
    }

    //Create Voxel Grid
    CompFab::Vec3 bbMax, bbMin;
    BBox(*tempMesh, bbMin, bbMax);
    
    //Build Voxel Grid
    double bbX = bbMax[0] - bbMin[0];
    double bbY = bbMax[1] - bbMin[1];
    double bbZ = bbMax[2] - bbMin[2];
    double spacing;
    
    if(bbX > bbY && bbX > bbZ)
    {
        spacing = bbX/(double)(dim-2);
    } else if(bbY > bbX && bbY > bbZ) {
        spacing = bbY/(double)(dim-2);
    } else {
        spacing = bbZ/(double)(dim-2);
    }
    
    CompFab::Vec3 hspacing(0.5*spacing, 0.5*spacing, 0.5*spacing);
    
    g_voxelGrid = new CompFab::VoxelGrid(bbMin-hspacing, dim, dim, dim, spacing);

    delete tempMesh;
    
    return true;
   
}

void saveVoxelsToObj(const char * outfile)
{
 
    Mesh box;
    Mesh mout;
    int nx = g_voxelGrid->m_dimX;
    int ny = g_voxelGrid->m_dimY;
    int nz = g_voxelGrid->m_dimZ;
    double spacing = g_voxelGrid->m_spacing;
    
    CompFab::Vec3 hspacing(0.5*spacing, 0.5*spacing, 0.5*spacing);
    
    for (int ii = 0; ii < nx; ii++) {
        for (int jj = 0; jj < ny; jj++) {
            for (int kk = 0; kk < nz; kk++) {
                if(!g_voxelGrid->isInside(ii,jj,kk)){
                    continue;
                }
                CompFab::Vec3 coord(0.5f + ((double)ii)*spacing, 0.5f + ((double)jj)*spacing, 0.5f+((double)kk)*spacing);
                CompFab::Vec3 box0 = coord - hspacing;
                CompFab::Vec3 box1 = coord + hspacing;
                makeCube(box, box0, box1);
                mout.append(box);
            }
        }
    }

    mout.save_obj(outfile);
}


int main(int argc, char **argv)
{
    // changing this!!
    unsigned int dim = 32; //dimension of voxel grid (e.g. 32x32x32)

    //Load OBJ
    if(argc < 3)
    {
        std::cout<<"Usage: Voxelizer InputMeshFilename OutputMeshFilename \n";
        exit(0);
    }
    
    std::cout<<"Load Mesh : "<<argv[1]<<"\n";
    loadMesh(argv[1], dim);
    

    
    //Cast ray, check if voxel is inside or outside
    //even number of surface intersections = outside (OUT then IN then OUT)
    // odd number = inside (IN then OUT)
    CompFab::Vec3 voxelPos;
    CompFab::Vec3 direction(1.0,0.0,0.0);
    
    /********* ASSIGNMENT *********/
    /* Iterate over all voxels in g_voxelGrid and test whether they are inside our outside of the
     * surface defined by the triangles in g_triangleList */
    
        int counter = 0; // global variable
        
        int nx = g_voxelGrid->m_dimX;
        int ny = g_voxelGrid->m_dimY;
        int nz = g_voxelGrid->m_dimZ;

        for (int x = 0; x < nx; x++)
        {
            for (int y = 0; y < ny; y++)
            {
                for (int z = 0; z < nz; z++)
                {   
                    voxelPos = g_voxelGrid->m_lowerLeft+CompFab::Vec3(x*g_voxelGrid->m_spacing, y*g_voxelGrid->m_spacing, z*g_voxelGrid->m_spacing);

                    int NumSurf = numSurfaceIntersections(voxelPos, direction);

                    if (NumSurf % 2 == 1)
                    {
                        //g_voxelGrid->SetInside(x, y, z); 
                        
                        g_voxelGrid->isInside(x, y, z) = true;
                    }
                    
                    counter++;

                    // else
                    // {
                    //     g_voxelGrid->isInside(x, y, z) = false;
                    // }

                }
            }
        }


    //Write out voxel data as obj
    saveVoxelsToObj(argv[2]);
    
    delete g_voxelGrid;
}