//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#version 140

in vec3 v2f_normal;
in vec2 v2f_texcoord;
in vec3 v2f_light;
in vec3 v2f_view;

out vec4 f_color;

uniform sampler2D day_texture;
uniform sampler2D night_texture;
uniform sampler2D cloud_texture;
uniform sampler2D gloss_texture;
uniform bool greyscale;

const float shininess = 20.0;
const vec3  sunlight = vec3(1.0, 0.941, 0.898);

void main()
{
    /** \todo
    * - Copy your working code from the fragment shader of your Phong shader use it as
    * starting point
    * - instead of using a single texture, use the four texures `day_texure`, `night_texure`,
    * `cloud_texure` and `gloss_texture` and mix them for enhanced effects
    * Hints:
    * - cloud and gloss textures are just greyscales. So you'll just need one color-
    * component.
    * - The texture(texture, 2d_position) returns a 4-vector (rgba). You can use
    * `texture(...).r` to get just the red component or `texture(...).rgb` to get a vec3 color
    * value
    * - use mix(vec3 a,vec3 b, s) = a*(1-s) + b*s for linear interpolation of two colors
     */
    vec3 color = vec3(0.0,0.0,0.0);
    vec3 r = normalize((2.0 * dot(v2f_light,v2f_normal)) * v2f_normal - v2f_light);
    
	//Combine gloss and cloudiness textures to get a grayscale value specifying the amount of specularity ([0,1]).
	float gloss = texture(gloss_texture,v2f_texcoord.st).r;
	float cloudiness = texture(cloud_texture,v2f_texcoord.st).r;
	float specularity = gloss * (1 - cloudiness);
     
    //Get a color of the day component by applying Phong lighting model (disregarding the clouds) and day texture.
    vec3 day_color = 0.2 * sunlight * texture(day_texture, v2f_texcoord.st).rgb;
    if (dot(v2f_normal, v2f_light) > 0)
    	day_color += sunlight * texture(day_texture, v2f_texcoord.st).rgb * dot(v2f_normal, v2f_light);
    
    if (dot(v2f_view, r) > 0)
    	day_color += sunlight * specularity * vec3(1,1,1) * pow(dot(v2f_view, r),shininess);
	
	//Get the final color of the day component by interpolating between the current day and clouds rendered with Lambertian lighting  model; interpolation weight is given by cloudness.
	vec3 cloud_color = 0.2 * sunlight * vec3(cloudiness,cloudiness,cloudiness);
    if (dot(v2f_normal, v2f_light) > 0)
    	cloud_color += sunlight * vec3(cloudiness,cloudiness,cloudiness) * dot(v2f_normal, v2f_light);
    	
	day_color = (1 - cloudiness) * day_color + cloudiness * cloud_color;
	
	//Get a color of the night component by sampling night texture and scale it down by cloudiness .
	vec3 night_color = (1 - cloudiness) * texture(night_texture, v2f_texcoord.st).rgb + cloudiness * vec3(0,0,0);
    
    //Mix day and night components by linear interpolation where you can use diffuse component of the Phong lighting model as the interpolation weight
    float weight = dot(v2f_normal, v2f_light);
	color = weight * day_color + (1 - weight) * night_color;
    
	
    // convert RGB color to YUV color and use only the luminance
    if (greyscale) color = vec3(0.299*color.r+0.587*color.g+0.114*color.b);

    // add required alpha value
    f_color = vec4(color,1.0);

}
