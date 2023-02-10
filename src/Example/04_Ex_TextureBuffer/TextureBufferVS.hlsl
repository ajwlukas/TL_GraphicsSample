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
float4 LocalToWorld(float3 localPos)//���� �������� �����
{
    return mul(world, float4(localPos, 1.0f));
}

float4 WorldToNDC(float4 worldPos)
{
    float4 ret = mul(view, worldPos);
    ret = mul(proj, ret);
    
    return ret;
}
float4 LocalToNDC(float3 localPos)//���� �������� �����
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
    float2 uv : UV;
    uint instanceID : SV_InstanceID;
};

struct VSout
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

Buffer<float4> transformMatrixes : register(t0);

float4x4 GetInstanceWorldTransform(uint instanceID)
{
    float4 r0 = transformMatrixes.Load(0 + 4 * instanceID);
    float4 r1 = transformMatrixes.Load(1 + 4 * instanceID);
    float4 r2 = transformMatrixes.Load(2 + 4 * instanceID);
    float4 r3 = transformMatrixes.Load(3 + 4 * instanceID);
    
    return float4x4(r0, r1, r2, r3);
}

float4 LocalToWorld(float3 localPos, uint instanceID)//���� �������� �����
{
    return mul(float4(localPos, 1.0f), GetInstanceWorldTransform(instanceID));
}

float4 LocalToNDC(float3 localPos, uint instanceID)//���� �������� �����
{
    float4 ret = LocalToWorld(localPos, instanceID);
    ret = WorldToNDC(ret);
    
    return ret;
}

///���Ϳ�
float3 LocalToWorldDirOnly(float3 localDir, uint instanceID)//���� �������� �����
{
    return mul(localDir, (float3x3) GetInstanceWorldTransform(instanceID));
}

float3 LocalToNDCDirOnly(float3 localDir, uint instanceID)//���� �������� �����
{
    float3 ret = LocalToWorldDirOnly(localDir, instanceID);
    ret = WorldToNDCDirOnly(ret);
    
    return ret;
}


VSout main(VSin input)
{
    VSout output;
    
    output.pos = LocalToNDC(input.pos, input.instanceID);
    output.uv = input.uv;
    
    return output;
}
