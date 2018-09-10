#version 440 core

out vec4 FragColor;
in vec2 VertexUv;

uniform sampler2D screenQuad;

const float PIXEL_OFFSET = 1.0f / 330.0f;
const float CONTRAST = 0.29f;

uniform bool blurEffect;
uniform bool grayScaleEffect;
uniform bool thunderstormEffect;

void main()
{	
	// Normal
	vec4 outputColor = texture(screenQuad, VertexUv);

	// Change contrast of texture so dark colors are slightly more darker and brighter colors are slightly brighter
	outputColor.rgb = (outputColor.rgb - 0.4f) * (1.0f + CONTRAST) + 0.4f;
	
	if (thunderstormEffect)
	{
		// Array of offsets for surrounding texture coordinates
		vec2 offsets[9] = vec2[]
		(
			vec2(-PIXEL_OFFSET, PIXEL_OFFSET),  // top left
			vec2( 0.0f, PIXEL_OFFSET), 			// top center
			vec2( PIXEL_OFFSET, PIXEL_OFFSET),  // top right
			vec2(-PIXEL_OFFSET, 0.0f),   		// center left
			vec2( 0.0f, 0.0f),   				// center
			vec2( PIXEL_OFFSET, 0.0f),   		// center right
			vec2(-PIXEL_OFFSET, -PIXEL_OFFSET), // bottom left
			vec2( 0.0f, -PIXEL_OFFSET), 		// bottom center
			vec2( PIXEL_OFFSET, -PIXEL_OFFSET)  // bottom right    
		);
		
		// A kernel filter that creates a simple blur effect
		float kernel[9] = float[]
		(
			0.0625f, 0.125f, 0.0625f,
			0.125f, 0.25f, 0.125f,
			0.0625f, 0.125f, 0.0625f
		);
		
		vec3 sampleTex[9];
		for(int i = 0; i < 9; ++i)
		{
			sampleTex[i] = vec3(texture(screenQuad, VertexUv.st + offsets[i]));
		}
		
		// Combine sampled colors
		for(int i = 0; i < 9; ++i)
		{
			outputColor.rgb += sampleTex[i] * kernel[i];
		}
		
		FragColor = vec4(outputColor.rgb, 1.0f);
	}
	
	if (blurEffect)
	{
		//// Array of offsets for surrounding texture coordinates
		//vec2 offsets[9] = vec2[]
		//(
		//	vec2(-PIXEL_OFFSET, PIXEL_OFFSET),  // top left
		//	vec2( 0.0f, PIXEL_OFFSET), 			// top center
		//	vec2( PIXEL_OFFSET, PIXEL_OFFSET),  // top right
		//	vec2(-PIXEL_OFFSET, 0.0f),   		// center left
		//	vec2( 0.0f, 0.0f),   				// center
		//	vec2( PIXEL_OFFSET, 0.0f),   		// center right
		//	vec2(-PIXEL_OFFSET, -PIXEL_OFFSET), // bottom left
		//	vec2( 0.0f, -PIXEL_OFFSET), 		// bottom center
		//	vec2( PIXEL_OFFSET, -PIXEL_OFFSET)  // bottom right    
		//);
		//
		//// A kernel filter that creates a simple blur effect
		//float kernel[9] = float[]
		//(
		//	0.0625f, 0.125f, 0.0625f,
		//	0.125f, 0.25f, 0.125f,
		//	0.0625f, 0.125f, 0.0625f
		//);
        //
		//vec3 sampleTex[9];
		//for(int i = 0; i < 9; ++i)
		//{
		//	sampleTex[i] = vec3(texture(screenQuad, VertexUv.st + offsets[i]));
		//}
		//
		//// Combine sampled colors
		//for(int i = 0; i < 9; ++i)
		//{
		//	outputColor.rgb += sampleTex[i] * kernel[i];
		//}
		//
		//FragColor = vec4(outputColor.rgb, 1.0f);
	}
	else if (grayScaleEffect)
	{
		// Grayscale: Remove all colors except black, white and gray (average all color components)
		FragColor = texture(screenQuad, VertexUv);
		float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0f;
		FragColor = vec4(average, average, average, 1.0f);
	}
	else
	{
		FragColor = vec4(outputColor.rgb, 1.0f);
	}
} 