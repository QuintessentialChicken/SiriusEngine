cbuffer CBuf {
    float4 face_colors[6];
};

float4 main(uint tid : SV_PrimitiveID) : SV_Target {
// Each triangle has a unique ID. Pick the color based on the triangle ID.
    return face_colors[tid / 2];
}