#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"
#include "shaderLoader.h"
#include "TextureLoading.h"
#include "VertexArraysObject.h"
#include "VertexBufferObjects.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

int main() {
	//начало
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.majorVersion = 4;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Core;

	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "SkyBox",
		sf::Style::Titlebar | sf::Style::Close, settings);

	window.setMouseCursorVisible(false);
	window.setMouseCursorGrabbed(true);

	glewExperimental = GL_TRUE;
	glEnable(GL_DEPTH_TEST);

	if (GLEW_OK != glewInit()) {
		std::cout << "Error:: glew not init =(" << std::endl;
		return -1;
	}
	//

	//камера
	Camera camera;
	//

	//шейдеры
	Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs");
    Shader modelShader("shaders/model.vs", "shaders/model.fs");
	//

    //модели
    Model modelSign("objects/clean/Half-Dirty.obj");
    Model modelBomb("objects/bomb/Bomb.obj");
    //
	
	//вершины скайбокса
    float skyboxVertices[] = {       
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    //позиции моделей
    glm::vec3 positionModel[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 0.0f, 2.0f),
    };
	//

    //скайбокс
    VertexArraysObject VAO;
    VAO.Binding();
    VertexBufferObjects VBO(skyboxVertices, sizeof(skyboxVertices) / sizeof(float));
    VAO.VertexAttribPointer(0, 3, 3 * sizeof(float), (void*)0);
    //

	//для каждой грани куба картинка
	std::vector<std::string> skybox
	{
		"skybox/right.png",
		"skybox/left.png",
		"skybox/top.png",
		"skybox/bottom.png",
		"skybox/front.png",
		"skybox/back.png"
	};
    //

    //текстура для скайбокca
	TextureLoading textureSkyBox(skybox);
    //

	skyboxShader.Use();
	skyboxShader.SetInt("skybox", 0);


	bool isGo = true;
	while (true) {
		//обработка движений камеры
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) {
			switch (windowEvent.type) {
			case sf::Event::Closed:
				isGo = false;
				break;
			case sf::Event::KeyPressed:
				camera.ReadInput();
				break;
			case sf::Event::MouseMoved:
				camera.MouseInput(window);
				break;
			default:
				break;
			}
		}
		//

		//
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//

        modelShader.Use();
        auto view = camera.GetViewMatrix();
        auto projection = camera.GetProjectionMatrix();
        modelShader.SetMat4("projection", projection);
        modelShader.SetMat4("view", view);

        //рендеринг моделей
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, positionModel[0]);
        modelShader.SetMat4("model", model);
        modelSign.Draw(modelShader);

        glm::mat4 model1 = glm::mat4(1.0f);
        model1 = glm::translate(model, positionModel[1]);
        model1 = glm::scale(model1, glm::vec3(2.0f));
        modelShader.SetMat4("model", model1);
        modelBomb.Draw(modelShader);
        //

        //скайбокс
        glDepthFunc(GL_LEQUAL);
        skyboxShader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyboxShader.SetMat4("view", view);
        skyboxShader.SetMat4("projection", projection);

        VAO.Binding();
        textureSkyBox.Binding(0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        VAO.Binding(0);
        glDepthFunc(GL_LESS);
        //

		window.display();
	}

	window.close();
	return 0;
}