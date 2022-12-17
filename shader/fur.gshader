#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 26) out;
in VS_OUT{
    vec3 Normal;
    vec3 FragPos;
} gs_in[];
uniform mat4 projection;

void generatefur(int posindex,float length){
    gl_Position = gl_in[posindex].gl_Position; 
    EmitVertex();
    gl_Position = gl_in[posindex].gl_Position + projection * vec4(normalize(gs_in[posindex].Normal),0.0f) *length;
    EmitVertex();
    EndPrimitive();
}

void generateinnerfur(float rate1, float rate2, float rate0, float length){
    gl_Position = gl_in[1].gl_Position*rate1 + gl_in[2].gl_Position*rate2 + gl_in[0].gl_Position*rate0; 
    EmitVertex();
    gl_Position = gl_in[1].gl_Position*rate1 + gl_in[2].gl_Position*rate2 + gl_in[0].gl_Position*rate0 + projection * vec4(normalize(gs_in[1].Normal)*rate1 +normalize(gs_in[2].Normal)*rate2 +normalize(gs_in[0].Normal)*rate0,0.0f) *length;
    EmitVertex();
    EndPrimitive();
}

void main(){
    generatefur(0,0.01f);
    generatefur(1,0.01f);
    generatefur(2,0.01f);
    generateinnerfur(0,0.33f,0.66f,0.008f);
    generateinnerfur(0,0.66f,0.33f,0.008f);
    generateinnerfur(0.33f,0.66f,0,0.008f);
    generateinnerfur(0.66f,0.33f,0,0.008f);
    generateinnerfur(0.33f,0,0.66f,0.008f);
    generateinnerfur(0.66f,0,0.33f,0.008f);
    generateinnerfur(0.33f,0.33f,0.33f,0.008f);
    generateinnerfur(0.16f,0.42f,0.42f,0.008f);
    generateinnerfur(0.42f,0.42f,0.16f,0.008f);
    generateinnerfur(0.42f,0.16f,0.42f,0.008f);
}