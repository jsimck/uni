#ifndef __TANGENTSPACE_H_
#define __TANGENTSPACE_H_

#include <glm.hpp>
#include <gtc/matrix_transform.hpp> 
#include <gtc/matrix_inverse.hpp>
#include <gtc/epsilon.hpp>
#include <gtx/norm.hpp>


class TangentSpace
{
public :
	void createNormalizedInverseTBN(glm::vec3 *v1, glm::vec3 *v2, glm::vec3 *v3,
									glm::vec3 *v1tex, glm::vec3 *v2tex, glm::vec3 *v3tex,
									glm::vec3 *vTangent, glm::vec3 *vBitangent, glm::vec3 *vNormal);

	void createNormalizedTBN(glm::vec3 *v1, glm::vec3 *v2, glm::vec3 *v3,
									glm::vec3 *v1tex, glm::vec3 *v2tex, glm::vec3 *v3tex,
									glm::vec3 *vTangent, glm::vec3 *vBitangent, glm::vec3 *vNormal);

	void createNormalizedT(glm::vec3 *v1, glm::vec3 *v2, glm::vec3 *v3,
						   glm::vec3 *v1tex, glm::vec3 *v2tex, glm::vec3 *v3tex,
						   glm::vec4 *vTangent);
};

inline void TangentSpace::createNormalizedInverseTBN(glm::vec3 *v1, glm::vec3 *v2, glm::vec3 *v3,
													 glm::vec3 *v1tex, glm::vec3 *v2tex, glm::vec3 *v3tex,
													 glm::vec3 *vTangent,glm::vec3 *vBitangent,glm::vec3 *vNormal)
{
	


	//Create edge vectors from vertex 1 to vectors 2 and 3.
    float x1 = v2->x - v1->x;
    float y1 = v2->y - v1->y;
    float z1 = v2->z - v1->z;
    float x2 = v3->x - v1->x;
    float y2 = v3->y - v1->y;
    float z2 = v3->z - v1->z;

    // Create edge vectors from the texture coordinates of vertex 1 to vector 2 and 3
    float s1 = v2tex->s - v1tex->s;
    float t1 = v2tex->t - v1tex->t;
    float s2 = v3tex->s - v1tex->s;
    float t2 = v3tex->t - v1tex->t;
		
    float fDenominator = s1 * t2 - s2 * t1;

    if( fDenominator < 0.0001f && fDenominator > -0.0001f )			// We're too close to zero and we're at risk of a divide-by-zero! Set the tangent matrix to the identity matrix and do nothing.
    {
        *vTangent  = glm::vec3( 1.0f, 0.0f, 0.0f );
        *vBitangent = glm::vec3( 0.0f, 1.0f, 0.0f );
        *vNormal   = glm::vec3( 0.0f, 0.0f, 1.0f );
    }
    else
    {
        // Calculate and cache the reciprocal value
        float fScale1 = 1.0f / fDenominator;

        glm::vec3 T = glm::vec3((t2 * x1 - t1 * x2) * fScale1,
							    (t2 * y1 - t1 * y2) * fScale1,
								(t2 * z1 - t1 * z2) * fScale1);

        glm::vec3 B = glm::vec3((-s2 * x1 + s1 * x2) * fScale1,
						        (-s2 * y1 + s1 * y2) * fScale1,
			                    (-s2 * z1 + s1 * z2) * fScale1);

        // The normal N is calculated as the cross product between T and B
		glm::vec3 N = glm::cross(T,B);

        //Inverse ...
		// Calculate and cache the reciprocal value
        float fScale2 = 1.0f / ((T.x * B.y * N.z - T.z * B.y * N.x) + 
                                (B.x * N.y * T.z - B.z * N.y * T.x) + 
                                (N.x * T.y * B.z - N.z * T.y * B.x));

        // Use the temporary T (Tangent), (B) Binormal, and N (Normal) vectors to calculate the inverse of the tangent matrix that they represent.
        // The inverse of the tangent matrix is what we want since we need that to transform the light's vector into tangent-space.
        (*vTangent).x =  T.x * fScale2;
        (*vTangent).y = -(B.x * fScale2);
        (*vTangent).z =  N.x * fScale2;
		(*vTangent) = glm::normalize(*vTangent);

        (*vBitangent).x = -(T.y * fScale2);
        (*vBitangent).y =  B.y * fScale2;
        (*vBitangent).z = -(N.y * fScale2);
        (*vBitangent) = glm::normalize(*vBitangent);

        (*vNormal).x =  T.z * fScale2;
        (*vNormal).y = -(B.z * fScale2);
        (*vNormal).z =  N.z * fScale2;
        (*vNormal) = glm::normalize((*vNormal));

        // NOTE: Since the texture-space of Direct3D and OpenGL are laid-out 
        //       differently, a single normal map can't look right in both 
        //       unless you make some adjustments somewhere.
        //
        //       You can adjust or fix this problem in three ways:
        //
        //       1. Create two normal maps: one for OpenGL and one for Direct3D.
        //       2. Flip the normal map image over as you load it into a texture 
        //          object.
        //       3. Flip the binormal over when computing the tangent-space
        //          matrix.
        //
        // Since the normal map used by this sample was created for Direct3D,
        // I've decided to simply flip the binormal.
        //
        *vBitangent = *vBitangent * -1.0f;
    }
}


inline void TangentSpace::createNormalizedTBN(glm::vec3 *v1, glm::vec3 *v2, glm::vec3 *v3,
													 glm::vec3 *v1tex, glm::vec3 *v2tex, glm::vec3 *v3tex,
													 glm::vec3 *vTangent,glm::vec3 *vBitangent,glm::vec3 *vNormal)
{
	//Create edge vectors from vertex 1 to vectors 2 and 3.
    float x1 = v2->x - v1->x;
    float y1 = v2->y - v1->y;
    float z1 = v2->z - v1->z;
    float x2 = v3->x - v1->x;
    float y2 = v3->y - v1->y;
    float z2 = v3->z - v1->z;

    // Create edge vectors from the texture coordinates of vertex 1 to vector 2 and 3
    float s1 = v2tex->s - v1tex->s;
    float t1 = v2tex->t - v1tex->t;
    float s2 = v3tex->s - v1tex->s;
    float t2 = v3tex->t - v1tex->t;
		
    float fDenominator = s1 * t2 - s2 * t1;

    if( fDenominator < 0.0001f && fDenominator > -0.0001f )			// We're too close to zero and we're at risk of a divide-by-zero! Set the tangent matrix to the identity matrix and do nothing.
    {
        *vTangent  = glm::vec3( 1.0f, 0.0f, 0.0f );
        *vBitangent = glm::vec3( 0.0f, 1.0f, 0.0f );
        *vNormal   = glm::vec3( 0.0f, 0.0f, 1.0f );
    }
    else
    {
        // Calculate and cache the reciprocal value
        float fScale1 = 1.0f / fDenominator;

        glm::vec3 T = glm::vec3((t2 * x1 - t1 * x2) * fScale1,
								(t2 * y1 - t1 * y2) * fScale1,
								(t2 * z1 - t1 * z2) * fScale1);

        glm::vec3 B = glm::vec3((-s2 * x1 + s1 * x2) * fScale1,
								(-s2 * y1 + s1 * y2) * fScale1,
								(-s2 * z1 + s1 * z2) * fScale1);

        // The normal N is calculated as the cross product between T and B
		glm::vec3 N = glm::cross(T,B);

		// Gram-Schmidt orthogonalize
		glm::vec3 T2 = T - N * glm::dot(N, T);
		(*vTangent) = glm::normalize(T2);
		(*vNormal) = glm::normalize(N);
			
		//glm::vec3 B2 = B - N * glm::dot(N, B) - T2 * glm::dot(T2,B);  // ... missing /T2^2
		//Calculate handedness ... we want righthanded system in OpenGL
		glm::vec3 B2 = glm::normalize(glm::cross(N,T));
		if (glm::dot(B2, B) < 0.0f)
			(*vBitangent) = glm::normalize(-B2);
		else
			(*vBitangent) = glm::normalize(B2);
    }
}


inline void TangentSpace::createNormalizedT(glm::vec3 *v1, glm::vec3 *v2, glm::vec3 *v3,
											glm::vec3 *v1tex, glm::vec3 *v2tex, glm::vec3 *v3tex,
											glm::vec4 *vTangent)
{
	//Create edge vectors from vertex 1 to vectors 2 and 3.
    float x1 = v2->x - v1->x;
    float y1 = v2->y - v1->y;
    float z1 = v2->z - v1->z;
    float x2 = v3->x - v1->x;
    float y2 = v3->y - v1->y;
    float z2 = v3->z - v1->z;

    // Create edge vectors from the texture coordinates of vertex 1 to vector 2 and 3
    float s1 = v2tex->s - v1tex->s;
    float t1 = v2tex->t - v1tex->t;
    float s2 = v3tex->s - v1tex->s;
    float t2 = v3tex->t - v1tex->t;
		
    float fDenominator = s1 * t2 - s2 * t1;

    if( fDenominator < 0.0001f && fDenominator > -0.0001f )			// We're too close to zero and we're at risk of a divide-by-zero! Set the tangent matrix to the identity matrix and do nothing.
    {
        *vTangent = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0 );
    }
    else
    {
        // Calculate and cache the reciprocal value
        float fScale1 = 1.0f / fDenominator;

        glm::vec3 T = glm::vec3((t2 * x1 - t1 * x2) * fScale1,
								(t2 * y1 - t1 * y2) * fScale1,
								(t2 * z1 - t1 * z2) * fScale1);

        glm::vec3 B = glm::vec3((-s2 * x1 + s1 * x2) * fScale1,
								(-s2 * y1 + s1 * y2) * fScale1,
								(-s2 * z1 + s1 * z2) * fScale1);

        // The normal N is calculated as the cross product between T and B
		glm::vec3 N = glm::cross(T,B);


		// Gram-Schmidt orthogonalize
		glm::vec3 T2 = (T - N * glm::dot(N, T));
		T = glm::normalize(T2);
		
		//It is not necessary to store an extra array containing the per-vertex bitangent since the cross product N × T2 can be used to obtain mB2, where m = ±1 represents the handedness of the tangent space.
		// The handedness value must be stored per-vertex since the bitangent B2 obtained from N × T2 may point in the wrong direction. The value of m is equal to the determinant of the matrix TBNi (=inverse TBN matrix). 
		// One may find it convenient to store the per-vertex tangent vector T2 as a four-dimensional entity whose w coordinate holds the value of m. Then the bitangent B2 can be computed using the formula
		// B2 = T2.xyz (N.xyz × T2.xyz),

		// Calculate handedness
		float m = (glm::dot(glm::cross(N, T), B) < 0.0f) ? -1.0f : 1.0f;	

		*vTangent = glm::vec4(T, m);
    }
}
#endif

