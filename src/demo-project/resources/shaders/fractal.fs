#version 460 core

out vec4 FragColor;

in vec3 ourColour;
in vec2 fragCoord;

// TODO: refactor this so it will work with an existing demo, just as a new shader
void mainImage() {
  // Normalized pixel coordinates (from 0 to 1)
  vec2 uv = fragCoord/iResolution.xy;
  uv *= 4.0;
  uv -= 2.0;
  
  vec2 c = vec2(-1.0, 0.0);
  vec2 z = uv;
  int iter = 60;

  
  // other fancy things
  //vec2 z = vec2(sin(iTime), cos(iTime));
  vec2 offset;
  if (iMouse.x > 20.0) {
      offset = iMouse.xy / iResolution.xy;
      offset *= 4.0;
      offset -= 2.0;
  }
  
  offset -= vec2(0.4+sin(iTime)/5.0, 0);
  float zoom = 1.0;
  
  c = c * zoom + offset;
  
  
  // z = x + y*i, this is a complex number
  // equation for mandlebrot is f(z) = z^2 + c
  // just iterate, and the output is when this goes bigger than 2
  
  
  
  float iterations = 0.0;

  for (int i = 0; i < iter; i++) {

      // z^2 = x^2 + 2xyi -y^2
      vec2 znew;
      znew.x = z.x*z.x - z.y*z.y + c.x + length(z);
      znew.y = 2.0*z.x*z.y + c.y;
      
      if (length(z) > 2.0) break;
      z = znew;
      iterations++;
  }
  

  // Time varying pixel color
  vec3 col = vec3(uv.xy, 0.0);

  // Output to screen
  vec3 t = vec3(iterations/float(iter));

  fragColor = vec4(t,1.0);
}