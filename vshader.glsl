attribute vec2 a_texcoord;
attribute vec4 a_position;
varying vec2 v_texcoord;

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
    float rr = r - 0.5 * r * r;
    // float rr = -sqrt(1.0 - 2.0 * r) + 1.0;
    vec2 pp = vec2(rr, theta);
    return toPoint(pp);
}

vec4 transform(vec4 pos) {
    vec2 p = vec2(pos.x, pos.y);
    p = fisheye(p);
    return vec4(p.x, p.y, pos.z, pos.w);
}

void main() {
    gl_Position = a_position;
    // gl_Position = transform(a_position);
    v_texcoord = a_texcoord;
}
