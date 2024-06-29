{
	name: "UI Shader",
	{
		stage: VERTEX,
		source: "
			#version 130
			#extension GL_ARB_explicit_attrib_location : enable
			layout (location = 0) in vec3 pos_a;

			// Instanced
			layout (location = 1) in vec3 offset_a;
			layout (location = 2) in vec2 scale_a;
			layout (location = 3) in vec3 color_a;
			layout (location = 4) in vec3 border_color_a;
			layout (location = 5) in vec4 border_a;
			layout (location = 6) in vec4 texture_src_a;

			uniform mat4 tex_coordinates;
			uniform mat4 projection;


			out vec2 tex_coordinate_v;
			out vec3 color_v;
			out vec3 border_color_v;
			out vec4 border_v;
			flat out vec4 texture_src_f;

			void main(){
				gl_Position = projection * vec4(pos_a * vec3(scale_a, 1) + offset_a, 1);

				int quadVert = gl_VertexID % 4;
				tex_coordinate_v = vec2(tex_coordinates[quadVert][0], tex_coordinates[quadVert][1]);
				color_v = color_a;
				border_color_v = border_color_a;
				border_v = border_a;
				texture_src_f = texture_src_a;
			}
		",
	},
	{
		stage: FRAGMENT,
		source: "
			#version 130

			out vec4 FragColor;

			in vec2 tex_coordinate_v;
			in vec3 color_v;
			in vec3 border_color_v;
			in vec4 border_v;
			flat in vec4 texture_src_f;

			uniform sampler2D src_texture;

			void main(){
				vec2 tex_coord = tex_coordinate_v;
				vec2 texture_size = textureSize(src_texture, 0);

				tex_coord *= texture_src_f.zw / texture_size;
				tex_coord += texture_src_f.xy / texture_size;

				FragColor = texture(src_texture, tex_coord) * vec4(color_v, 1);
				if(FragColor.a < 0.01){
					discard;
				}
			}
		",
	},
}
