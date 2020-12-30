#ifndef PG1_MACROS_H
#define PG1_MACROS_H

#define ALIGNMENT 16
#define ALIGN __declspec( align( ALIGNMENT ) )
#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#define REAL_MAX static_cast<float>( FLT_MAX )
#define REAL_MIN static_cast<float>( -FLT_MAX )
#define EPSILON static_cast<float>( 1e-5 )

#define DEG2RAD(x) ( ( x ) * static_cast<float>( M_PI / 180 ) )
#define RAD2DEG(x) ( ( x ) * static_cast<float>( 180 / M_PI ) )
#define SQR(x) ( ( x ) * ( x ) )

#ifndef MIN
#define MIN( a, b ) ( ( a < b )? ( a ) : ( b ) )
#endif

#ifndef MAX
#define MAX( a, b ) ( ( a > b )? ( a ) : ( b ) )
#endif

#endif
