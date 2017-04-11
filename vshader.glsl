#version 150

in vec3 vPosition;
in vec3 vnormal;
//in vec4 vColor;
out vec4 color;

uniform vec3 theta;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 ldir;
uniform vec3 eyepos;
//vec3 diffuse=vec3(1,0,0);
vec3 ambient=vec3(0,0,1);
vec4 specular=vec4(1,1,1,1);
float shininess=2;
vec3 diffuse=vec3(1,0.5,0.1);



void main()
{
	vec3 angles = radians( theta );
	vec3 c = cos( angles );
	vec3 s = sin( angles );

    // Remember: this matrix is a column-major
	mat4 rx = mat4( 1.0, 0.0, 0.0, 0.0,
					0.0, c.x, -s.x, 0.0,
					0.0, s.x, c.x, 0.0,
					0.0, 0.0, 0.0, 1.0 );
					mat4 ry = mat4( c.y, 0.0, -s.y, 0.0,
					0.0, 1, 0.0, 0.0,
					s.y, 0.0, c.y, 0.0,
					0.0, 0.0, 0.0, 1.0 );
					mat4 rz = mat4( c.z, -s.z, 0.0, 0.0,
					s.z, c.z, 0.0, 0.0,
					0.0, 0.0, 1, 0.0,
					0.0, 0.0, 0.0, 1.0 );
    
	
	
	// set the specular term initially to black
  // vec4 spec = vec4(0.0);
   vec4 spec;

   // transform normal to camera space and normalize it
    vec3 n =vec3 (normalize(view * model* vec4(vnormal,1)));

	 // compute the intensity as the dot product
    // the max prevents negative intensity values
    float intensity = max(dot(n, ldir), 0.0);

	 // if the vertex is lit compute the specular term
    if (intensity > 0.0) {
        // compute position in camera space
        vec3 pos = vec3(view*model * vec4(vPosition,1));
        // compute eye vector and normalize it 
        vec3 eye = normalize(-pos);
        // compute the half vector
        vec3 h = normalize(ldir - eye);
 
        // compute the specular term into spec
        float intSpec = max(dot(h,n), 0.0);
        spec = specular * pow(intSpec, shininess);
    }

	//try vertex position as a diffuse material value
	//diffuse+=vPosition;

	 // Compute the color per vertex
     // color = max(intensity * vec4(diffuse,1)+spec,vec4(ambient,1));
     // color = (spec);
      
      color = max(intensity * vec4(diffuse,1),vec4(ambient,1));
      // color = (intensity * vec4(diffuse,1));

	   gl_Position = (projection*view*model)*vec4(vPosition,1);
 

	/*color =  vec4(vPosition.r+.3,vPosition.g+.3,vPosition.b+.3, 1.0);
	gl_Position = scale*ry*vec4(vPosition, 1.0);*/



}