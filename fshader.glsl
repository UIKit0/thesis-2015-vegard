varying vec2 v_texcoord;
uniform sampler2D s_texture;

void main()
{
    // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    gl_FragColor = texture2D(s_texture, v_texcoord);
}
