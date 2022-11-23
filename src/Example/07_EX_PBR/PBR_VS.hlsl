cbuffer CameraBuffer : register(b0)
{
    float4x4 view;
    float4x4 proj;
    float3 camPos;
}

cbuffer World : register(b1)
{
    float4x4 world;
};

///���ؽ���
float4 LocalToWorld(float4 localPos)//���� �������� �����
{
    return mul(world, localPos);
}

float4 WorldToNDC(float4 worldPos)
{
    float4 ret = mul(view, worldPos);
    ret = mul(proj, ret);
    
    return ret;
}
float4 LocalToNDC(float4 localPos)//���� �������� �����
{
    float4 ret = LocalToWorld(localPos);
    ret = WorldToNDC(ret);
    
    return ret;
}

///���Ϳ�
float3 LocalToWorldDirOnly(float3 localDir)//���� �������� �����
{
    return mul((float3x3) world, localDir);
}

float3 WorldToNDCDirOnly(float3 worldDir)
{
    float3 ret = mul((float3x3) view, worldDir);
    ret = mul((float3x3) proj, ret);
    
    return ret;
}

float3 LocalToNDCDirOnly(float3 localDir)//���� �������� �����
{
    float3 ret = LocalToWorldDirOnly(localDir);
    ret = WorldToNDCDirOnly(ret);
    
    return ret;
}

struct VSin
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
};

struct VSout
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float3 posW : POSITIONW;
};

VSout main(VSin input)
{
    VSout output;
    
    output.posW = LocalToWorld(float4(input.pos, 1.0f));
    output.pos = WorldToNDC(float4(output.posW, 1.0f));
    output.normal = LocalToWorldDirOnly(input.normal);
    
    return output;
}
