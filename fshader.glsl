varying vec2 v_texcoord;
uniform sampler2D s_texture;

vec2 postotexcoord(vec2 pos) {
    return vec2(pos.x + 0.5, pos.y + 0.5);
}

vec2 texcoordtopos(vec2 tex) {
    return vec2(tex.x - 0.5, tex.y - 0.5);
}

vec2 toPolar(vec2 point) {
    float r = length(point);
    float theta = atan(point.y, point.x);
    return vec2(r, theta);
}

vec2 toPoint(vec2 pol) {
    float r = pol.x;
    float theta = pol.y;
    float x = r * cos(theta);
    float y = r * sin(theta);
    return vec2(x, y);
}

vec2 fisheye(vec2 pos) {
    vec2 p = toPolar(pos);
    float r = p.x;
    float theta = p.y;
    // float rr = r - 0.5 * r * r;
    float rr = -sqrt(1.0 - 2.0 * r) + 1.0;
    vec2 pp = vec2(rr, theta);
    return toPoint(pp);
}

vec2 transform(vec2 tex) {
    return postotexcoord(fisheye(texcoordtopos(tex)));
}

void main() {
    // gl_FragColor = texture2D(s_texture, v_texcoord);
    gl_FragColor = texture2D(s_texture, transform(v_texcoord));
}
