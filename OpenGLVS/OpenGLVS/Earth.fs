#version 330 core
out vec4 FragColor;

struct Material {
    samplerCube diffuse;
    samplerCube specular;
    samplerCube night;   
    float shininess;
}; 

struct Light {
    vec4 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};
   
in vec3 TexCoord;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform bool Sunlight;
uniform bool Clouds;

void main()
{    
    float clouds = length(texture(material.diffuse, TexCoord))/2;   

    // ambient
    vec4 ambient = light.ambient * texture(material.diffuse, TexCoord);
    if(Sunlight)	//For bright radiance of Sun
    ambient = texture(material.diffuse, TexCoord);
    if(Clouds)		//for cloud rendering
{
    if(clouds>0.55f)	
    ambient = light.ambient * vec4(vec3(2*texture(material.diffuse, TexCoord).rgb), clouds);
    else
    discard;
}
  	
    // diffuse 
    vec4 norm = normalize(vec4(TexCoord,0.0f));
    vec4 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord);  
    if(Sunlight)
    diffuse = texture(material.diffuse, TexCoord);
    if(Clouds)
{
    if(clouds>0.55f)	
    diffuse = light.diffuse * diff * vec4(vec3(2*texture(material.diffuse, TexCoord).rgb), clouds);
    else
    discard;
}
    
    // specular
    vec4 viewDir = vec4(normalize(viewPos - TexCoord),0.0f);
    vec4 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); 
    vec4 specular = light.specular * spec * texture(material.specular, TexCoord);
    if(Sunlight)
    specular = vec4(1.0f, 140.0f/255.0f , 0.0f, 1.0f) * texture(material.specular, TexCoord);
    if(Clouds)
    specular = vec4(0.0f,0.0f,0.0f,1.0f);
        
    vec4 result = ambient + diffuse + specular;

    if(dot(norm, lightDir) > 0.1 || Sunlight || Clouds)	
    FragColor = vec4(result);
    else if(dot(norm, lightDir) <=0.1 && dot(norm, lightDir) >=-0.1)	//Blending between night and day
{
    for(float i=-0.1;i<=0.1;i=i+0.001)
    if(dot(norm, lightDir) <=i+0.001 && dot(norm, lightDir) >=i)
    FragColor = mix(texture(material.night, TexCoord), result,5*i+0.5f );
}
    else
    FragColor = texture(material.night, TexCoord); 
}