#version 330 core
out vec4 FragColor;

in vec4 vColor;
in vec2 vTexCoord;
in float vTexIndex;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;
uniform sampler2D uTexture4;
uniform sampler2D uTexture5;
uniform sampler2D uTexture6;
uniform sampler2D uTexture7;
uniform sampler2D uTexture8;
uniform sampler2D uTexture9;
uniform sampler2D uTexture10;
uniform sampler2D uTexture11;
uniform sampler2D uTexture12;
uniform sampler2D uTexture13;
uniform sampler2D uTexture14;
uniform sampler2D uTexture15;

void main() {
   int index = int(vTexIndex);
   if (index == -1) {
      FragColor = vColor;
   }
   if (index == 0) {
      FragColor = texture(uTexture0, vTexCoord);
   }
   if (index == 1) {
      FragColor = texture(uTexture1, vTexCoord);
   }
   if (index == 2) {
      FragColor = texture(uTexture2, vTexCoord);
   }
   if (index == 3) {
      FragColor = texture(uTexture3, vTexCoord);
   }
   if (index == 4) {
      FragColor = texture(uTexture4, vTexCoord);
   }
   if (index == 5) {
      FragColor = texture(uTexture5, vTexCoord);
   }
   if (index == 6) {
      FragColor = texture(uTexture6, vTexCoord);
   }
   if (index == 7) {
      FragColor = texture(uTexture7, vTexCoord);
   }
   if (index == 8) {
      FragColor = texture(uTexture8, vTexCoord);
   }
   if (index == 9) {
      FragColor = texture(uTexture9, vTexCoord);
   }
   if (index == 10) {
      FragColor = texture(uTexture10, vTexCoord);
   }
   if (index == 11) {
      FragColor = texture(uTexture11, vTexCoord);
   }
   if (index == 12) {
      FragColor = texture(uTexture12, vTexCoord);
   }
   if (index == 13) {
      FragColor = texture(uTexture13, vTexCoord);
   }
   if (index == 14) {
      FragColor = texture(uTexture14, vTexCoord);
   }
   if (index == 15) {
      FragColor = texture(uTexture15, vTexCoord);
   }
}
