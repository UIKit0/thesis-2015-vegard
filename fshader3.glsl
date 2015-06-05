varying vec2 v_texcoord;
uniform sampler2D s_texture;

vec4 color(sampler2D texture, vec2 pos) {
    if(pos.x < 0.0 || pos.y < 0.0 || pos.x > 1.0 || pos.y > 1.0) {
        return vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        return texture2D(texture, pos);
    }
}

vec2 clamp(vec2 pos) {
    float x = pos.x;
    float y = pos.y;

    if(x < 0.0 || y < 0.0) {
        x = 0.0;
        y = 0.0;
    }

    if(x > 1.0 || y > 1.0) {
        x = 1.0;
        y = 1.0;
    }

    return vec2(x, y);
}

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
    float s = 0.3390;
    float lambda = 3.8342;
    float rr = (exp(r / s) - 1.0) / lambda;
    vec2 pp = vec2(rr, theta);
    return toPoint(pp);
}

vec2 transform(vec2 tex) {
    return postotexcoord(fisheye(texcoordtopos(tex)));
}

vec4 blend5(vec4 c1, vec4 c2, vec4 c3, vec4 c4, vec4 c5) {
    return (c1 + c2 + c3 + c4 + c5) / 5.0;
}

vec4 blend9(vec4 c1, vec4 c2, vec4 c3, vec4 c4, vec4 c5, vec4 c6, vec4 c7, vec4 c8, vec4 c9) {
    return (c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8 + c9) / 9.0;
}

void main() {
    float px = 1.0/200.0; // fragment size

    // v0 v1 v2
    // v3 v4 v5
    // v6 v7 v8

    vec2 v0 = vec2(v_texcoord.x - px/2.0, v_texcoord.y + px/2.0);
    vec2 v1 = vec2(v_texcoord.x, v_texcoord.y + px/2.0);
    vec2 v2 = vec2(v_texcoord.x + px/2.0, v_texcoord.y + px/2.0);
    vec2 v3 = vec2(v_texcoord.x - px/2.0, v_texcoord.y);
    vec2 v4 = v_texcoord;
    vec2 v5 = vec2(v_texcoord.x + px/2.0, v_texcoord.y);
    vec2 v6 = vec2(v_texcoord.x - px/2.0, v_texcoord.y - px/2.0);
    vec2 v7 = vec2(v_texcoord.x, v_texcoord.y - px/2.0);
    vec2 v8 = vec2(v_texcoord.x + px/2.0, v_texcoord.y - px/2.0);

    v0 = transform(v0);
    v1 = transform(v1);
    v2 = transform(v2);
    v3 = transform(v3);
    v4 = transform(v4);
    v5 = transform(v4);
    v6 = transform(v4);
    v7 = transform(v4);
    v8 = transform(v4);

    vec4 c0 = color(s_texture, v0);
    vec4 c1 = color(s_texture, v1);
    vec4 c2 = color(s_texture, v2);
    vec4 c3 = color(s_texture, v3);
    vec4 c4 = color(s_texture, v4);
    vec4 c5 = color(s_texture, v4);
    vec4 c6 = color(s_texture, v4);
    vec4 c7 = color(s_texture, v4);
    vec4 c8 = color(s_texture, v4);

    // vec4 cx = c4; // disable supersampling
    // vec4 cx = blend5(c0, c2, c4, c6, c8);
    vec4 cx = blend9(c0, c1, c2, c3, c4, c5, c6, c7, c8);

    gl_FragColor = cx;
}
