#version 330

uniform sampler2D textureMap0; 

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec2 iTexCoord0;
in vec4 ic; 
in vec4 n1;
in vec4 l1;
in vec4 v1;
in vec4 n2;
in vec4 l2;
in vec4 v2;

void main(void) {

	//Znormalizowane interpolowane wektory
	vec4 ml1 = normalize(l1);
	vec4 mn1 = normalize(n1);
	vec4 mv1 = normalize(v1);
	//Wektor odbity
	vec4 mr1 = reflect(-ml1, mn1);

	//Znormalizowane interpolowane wektory
	vec4 ml2 = normalize(l2);
	vec4 mn2 = normalize(n2);
	vec4 mv2 = normalize(v2);
	//Wektor odbity
	vec4 mr2 = reflect(-ml2, mn2);

	//Parametry powierzchni
	vec4 kd = texture(textureMap0, iTexCoord0);
	vec4 ks = vec4(1, 1, 1, 1);

	//Obliczenie modelu oświetlenia
	float nl1 = clamp(dot(mn1, ml1), 0, 1);
	float rv1 = pow(clamp(dot(mr1, mv1), 0, 1), 50);
	float nl2 = clamp(dot(mn2, ml2), 0, 1);
	float rv2 = pow(clamp(dot(mr2, mv2), 0, 1), 50);
	pixelColor= vec4(kd.rgb * (nl1 + nl2), kd.a) + vec4(ks.rgb* (rv1 + rv2), 0);
}
