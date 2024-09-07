#shader vertex
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

out vec3 vert_color;

void main()
{
   vert_color = color;
   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}

#shader fragment
#version 330 core
in vec3 vert_color;
out vec4 frag_color;

void main()
{
    frag_color = vec4(vert_color, 1.0f);
<<<<<<< HEAD
}
=======
}

/*#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

in vec3 vert_color[];  // Receive color from vertex shader
out vec3 geom_color;   // Pass color to fragment shader

uniform float lineWidth;

void main()
{
    // Calculate normal vectors for the edges
    vec2 edge1 = normalize(gl_in[1].gl_Position.xy - gl_in[0].gl_Position.xy);
    vec2 edge2 = normalize(gl_in[2].gl_Position.xy - gl_in[1].gl_Position.xy);
    vec2 edge3 = normalize(gl_in[0].gl_Position.xy - gl_in[2].gl_Position.xy);

    // Calculate perpendicular offsets
    vec2 normal1 = vec2(-edge1.y, edge1.x);
    vec2 normal2 = vec2(-edge2.y, edge2.x);
    vec2 normal3 = vec2(-edge3.y, edge3.x);

    // Emit vertices for the first triangle
    geom_color = vert_color[0];
    gl_Position = gl_in[0].gl_Position + vec4(normal1 * lineWidth, 0.0, 0.0);
    EmitVertex();

    geom_color = vert_color[1];
    gl_Position = gl_in[1].gl_Position + vec4(normal1 * lineWidth, 0.0, 0.0);
    EmitVertex();

    geom_color = vert_color[2];
    gl_Position = gl_in[2].gl_Position + vec4(normal3 * lineWidth, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();

    // Emit vertices for the second triangle
    geom_color = vert_color[1];
    gl_Position = gl_in[1].gl_Position + vec4(normal2 * lineWidth, 0.0, 0.0);
    EmitVertex();

    geom_color = vert_color[2];
    gl_Position = gl_in[2].gl_Position + vec4(normal2 * lineWidth, 0.0, 0.0);
    EmitVertex();

    geom_color = vert_color[0];
    gl_Position = gl_in[0].gl_Position + vec4(normal3 * lineWidth, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}*/
>>>>>>> 7537bd658c6f3a2b5a2080cc93f14f916254bc56
