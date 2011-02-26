#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>

#include "android_glimp.h"
#include "../client/client.h"
#include "../renderer/tr_local.h"

static int screen_width=0;
static int screen_height=0;

static qboolean GLimp_HaveExtension(const char *ext)
{
	const char *ptr = Q_stristr( glConfig.extensions_string, ext );
	if (ptr == NULL)
		return qfalse;
	ptr += strlen(ext);
	return ((*ptr == ' ') || (*ptr == '\0'));  // verify it's complete string.
}

static void qglMultiTexCoord2f(GLenum target, GLfloat s, GLfloat t)
{
	qglMultiTexCoord4f(target,s,t,1,1);
}

/*
===============
GLimp_InitExtensions
===============
*/
static void GLimp_InitExtensions( void )
{
	if ( !r_allowExtensions->integer )
	{
		ri.Printf( PRINT_ALL, "* IGNORING OPENGL EXTENSIONS *\n" );
		return;
	}

	ri.Printf( PRINT_ALL, "Initializing OpenGL extensions\n" );

	glConfig.textureCompression = TC_NONE;

	// GL_EXT_texture_compression_s3tc
	if ( GLimp_HaveExtension( "GL_ARB_texture_compression" ) &&
	     GLimp_HaveExtension( "GL_EXT_texture_compression_s3tc" ) )
	{
		if ( r_ext_compressed_textures->value )
		{
			glConfig.textureCompression = TC_S3TC_ARB;
			ri.Printf( PRINT_ALL, "...using GL_EXT_texture_compression_s3tc\n" );
		}
		else
		{
			ri.Printf( PRINT_ALL, "...ignoring GL_EXT_texture_compression_s3tc\n" );
		}
	}
	else
	{
		ri.Printf( PRINT_ALL, "...GL_EXT_texture_compression_s3tc not found\n" );
	}

	// GL_S3_s3tc ... legacy extension before GL_EXT_texture_compression_s3tc.
	if (glConfig.textureCompression == TC_NONE)
	{
		if ( GLimp_HaveExtension( "GL_S3_s3tc" ) )
		{
			if ( r_ext_compressed_textures->value )
			{
				glConfig.textureCompression = TC_S3TC;
				ri.Printf( PRINT_ALL, "...using GL_S3_s3tc\n" );
			}
			else
			{
				ri.Printf( PRINT_ALL, "...ignoring GL_S3_s3tc\n" );
			}
		}
		else
		{
			ri.Printf( PRINT_ALL, "...GL_S3_s3tc not found\n" );
		}
	}


	// GL_EXT_texture_env_add
	glConfig.textureEnvAddAvailable = qtrue; //qfalse;
#if 0
	if ( GLimp_HaveExtension( "EXT_texture_env_add" ) )
	{
		if ( r_ext_texture_env_add->integer )
		{
			glConfig.textureEnvAddAvailable = qtrue;
			ri.Printf( PRINT_ALL, "...using GL_EXT_texture_env_add\n" );
		}
		else
		{
			glConfig.textureEnvAddAvailable = qfalse;
			ri.Printf( PRINT_ALL, "...ignoring GL_EXT_texture_env_add\n" );
		}
	}
	else
	{
		ri.Printf( PRINT_ALL, "...GL_EXT_texture_env_add not found\n" );
	}
#endif

	// GL_ARB_multitexture
	/*
	qglMultiTexCoord2fARB = NULL;
	qglActiveTextureARB = NULL;
	qglClientActiveTextureARB = NULL;
	*/
	//if ( GLimp_HaveExtension( "GL_ARB_multitexture" ) )
	{
		if ( r_ext_multitexture->value )
		{
			qglMultiTexCoord2fARB = qglMultiTexCoord2f;
			qglActiveTextureARB = qglActiveTexture;
			qglClientActiveTextureARB = qglClientActiveTexture;

			if ( qglActiveTextureARB )
			{
				GLint glint = 0;
				qglGetIntegerv( GL_MAX_TEXTURE_UNITS, &glint );
				glConfig.numTextureUnits = (int) glint;
				if ( glConfig.numTextureUnits > 1 )
				{
					ri.Printf( PRINT_ALL, "...using GL_ARB_multitexture\n" );
				}
				else
				{
					qglMultiTexCoord2fARB = NULL;
					qglActiveTextureARB = NULL;
					qglClientActiveTextureARB = NULL;
					ri.Printf( PRINT_ALL, "...not using GL_ARB_multitexture, < 2 texture units\n" );
				}
			}
		}
		else
		{
			ri.Printf( PRINT_ALL, "...ignoring GL_ARB_multitexture\n" );
		}
	}
#if 0
	else
	{
		ri.Printf( PRINT_ALL, "...GL_ARB_multitexture not found\n" );
	}
#endif

#if 0
	// GL_EXT_compiled_vertex_array
	if ( GLimp_HaveExtension( "GL_EXT_compiled_vertex_array" ) )
	{
		if ( r_ext_compiled_vertex_array->value )
		{
			ri.Printf( PRINT_ALL, "...using GL_EXT_compiled_vertex_array\n" );
			qglLockArraysEXT = ( void ( APIENTRY * )( GLint, GLint ) ) SDL_GL_GetProcAddress( "glLockArraysEXT" );
			qglUnlockArraysEXT = ( void ( APIENTRY * )( void ) ) SDL_GL_GetProcAddress( "glUnlockArraysEXT" );
			if (!qglLockArraysEXT || !qglUnlockArraysEXT)
			{
				ri.Error (ERR_FATAL, "bad getprocaddress");
			}
		}
		else
		{
			ri.Printf( PRINT_ALL, "...ignoring GL_EXT_compiled_vertex_array\n" );
		}
	}
	else
#endif
	{
		ri.Printf( PRINT_ALL, "...GL_EXT_compiled_vertex_array not found\n" );
	}

	textureFilterAnisotropic = qfalse;
#if 0
	if ( GLimp_HaveExtension( "GL_EXT_texture_filter_anisotropic" ) )
	{
		if ( r_ext_texture_filter_anisotropic->integer ) {
			qglGetIntegerv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, (GLint *)&maxAnisotropy );
			if ( maxAnisotropy <= 0 ) {
				ri.Printf( PRINT_ALL, "...GL_EXT_texture_filter_anisotropic not properly supported!\n" );
				maxAnisotropy = 0;
			}
			else
			{
				ri.Printf( PRINT_ALL, "...using GL_EXT_texture_filter_anisotropic (max: %i)\n", maxAnisotropy );
				textureFilterAnisotropic = qtrue;
			}
		}
		else
		{
			ri.Printf( PRINT_ALL, "...ignoring GL_EXT_texture_filter_anisotropic\n" );
		}
	}
	else
#endif
	{
		ri.Printf( PRINT_ALL, "...GL_EXT_texture_filter_anisotropic not found\n" );
	}
}

void GLimp_Init(void)
{
	ri.Printf(PRINT_ALL, "Initializing OpenGL subsystem\n");

	bzero(&glConfig, sizeof(glConfig));

	glConfig.isFullscreen = r_fullscreen->integer;
	glConfig.vidWidth = screen_width;
	glConfig.vidHeight = screen_height;
	glConfig.windowAspect = (float)glConfig.vidWidth / glConfig.vidHeight;
	// FIXME
	glConfig.colorBits = 16;
	glConfig.stencilBits = 8;
	glConfig.depthBits = 24;
	glConfig.textureCompression = TC_NONE;

	// This values force the UI to disable driver selection
	glConfig.driverType = GLDRV_ICD;
	glConfig.hardwareType = GLHW_GENERIC;

	Q_strncpyz(glConfig.vendor_string,
		   (const char *)qglGetString(GL_VENDOR),
		   sizeof(glConfig.vendor_string));
	Q_strncpyz(glConfig.renderer_string,
		   (const char *)qglGetString(GL_RENDERER),
		   sizeof(glConfig.renderer_string));
	Q_strncpyz(glConfig.version_string,
		   (const char *)qglGetString(GL_VERSION),
		   sizeof(glConfig.version_string));
	Q_strncpyz(glConfig.extensions_string,
		   (const char *)qglGetString(GL_EXTENSIONS),
		   sizeof(glConfig.extensions_string));

	qglLockArraysEXT = qglLockArrays;
	qglUnlockArraysEXT = qglUnlockArrays;

	GLimp_InitExtensions();

	IN_Init( );

	ri.Printf(PRINT_ALL, "------------------\n");
}

void GLimp_LogComment(char *comment)
{
	//fprintf(stderr, "%s: %s\n", __func__, comment);
//	ri.Printf(PRINT_ALL, "%s: %s\n", __func__, comment);
}

void GLimp_EndFrame(void)
{
}

void GLimp_Shutdown(void)
{
}

void qglArrayElement(GLint i)
{
}

void qglCallList(GLuint list)
{
}

void qglDrawBuffer(GLenum mode)
{
}

void qglLockArrays(GLint i, GLsizei size)
{
}

void qglUnlockArrays(void)
{
}

void GLimp_SetGamma(unsigned char red[256], unsigned char green[256],
		    unsigned char blue[256])
{
}

qboolean GLimp_SpawnRenderThread(void (*function) (void))
{
	return qfalse;
}

void GLimp_FrontEndSleep(void)
{
}

void *GLimp_RendererSleep(void)
{
	return NULL;
}

void GLimp_RenderThreadWrapper(void *data)
{
}

void GLimp_WakeRenderer(void *data)
{
}

void setResolution(int width, int height)
{
	screen_width = width;
	screen_height = height;
}
