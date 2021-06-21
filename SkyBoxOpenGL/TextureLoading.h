#pragma once
#include<iostream>
#include <string>
#include <vector>

class TextureLoading {
private:
	unsigned int texture;
	std::string type;
	std::string path;
public:
	TextureLoading() = default;

	TextureLoading(const char* path);

	TextureLoading(const char* path, const std::string& directory);

	TextureLoading(std::vector<std::string> texture_faces);

	void Binding(int check = -1);

	unsigned int GetTex();

	std::string GetType();

	std::string GetPath();

	void SetType(std::string type);

	void SetPath(std::string path);

};