#include "Headers/Screen.h"
#include "Headers/ImageOperations.h"

Screen* Screen::s_Instance = nullptr;

void Screen::Init()
{
	//Operation select options
	operations[0] = "Addition";
	operations[1] = "Subtraction";
	operations[2] = "Multiply";
	operations[3] = "Inversion";
	operations[4] = "Log Transform";
	operations[5] = "Power (Gamma) Transform";

	//user input filename
	file1 = new char[256]{ 0 };
	file2 = new char[256]{ 0 };

	img1 = ppmLoader::GetInstance()->LoadPPM("res/images/cameraman.ppm");
	tex1.Init(&img1);
	plane.Init();

	//Shader setup
	shader		= new Shader("res/shaders/Basic.shader");
	shader->Bind();

	tex1.Bind();
	shader->SetUniform1i("u_Texture", 0);

	//Static values
	projection	= glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT, -1.0f, 1.0f);
	view		= glm::translate(glm::mat4(1.0f), glm::vec3(WIDTH/2.0f, HEIGHT/2.0, 0));

	shader->SetUniformMat4f("u_MVP", MVP);

	shader->UnBind();
}

void Screen::Draw()
{	
	projection = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight, -1.0f, 1.0f);
	view = glm::translate(glm::mat4(1.0f), glm::vec3(windowWidth / 2.0f, windowHeight / 2.0, 0));
	//Update window propertiesf
	shader->Bind();

	//Update plane transforms
	plane.SetPosition({ img1_position[0], img1_position[1] });
	plane.SetScale({ img1_size, img1_size});
	MVP			= projection * view * plane.UpdateModelMatrix();

	shader->SetUniformMat4f("u_MVP", MVP);

	//Draws
	plane.Draw(*shader);

	if (operateImg)
	{
		//if artimetic operation
		if (currentOperation < 4)
		{
			img1 = ppmLoader::GetInstance()->LoadPPM("res/images/" + std::string(file1) + ".ppm");
			img2 = ppmLoader::GetInstance()->LoadPPM("res/images/" + std::string(file2) + ".ppm");
			if (img1.totalPixels == 0 || img2.totalPixels == 0)
				std::cout << "One or both file not found!!" << std::endl;
			else
			{
				std::cout << "Image processed" << std::endl;
				OperateImg();
				hasResult = true;
			}
		}
		else if (currentOperation >= 4)
		{
			img1 = ppmLoader::GetInstance()->LoadPPM("res/images/" + std::string(file1) + ".ppm");
			if (img1.totalPixels == 0)
				std::cout << "One or both file not found!!" << std::endl;
			else
			{
				std::cout << "Image processed" << std::endl;
				outImage = ImageTransform(&img1, currentOperation, intensity, gamma);
				hasResult = true;
			}
		}

		operateImg = false;
	}

	if (displayResult)
	{
		//display result image
		//output file
		hasResult = false;
		displayResult = false;
		ppmLoader::GetInstance()->writePPM("result_0", &outImage);
		tex1.Clean();
		tex1.Init(&outImage);
		tex1.Bind();
	}

	shader->UnBind();
}

void Screen::DrawUI()
{
	ImGui::Text("Image Transform");
	ImGui::Spacing();

	ImGui::Text("Position (x, y)");
	ImGui::SliderFloat2("pos", img1_position, -500.0f, 500.0f);

	ImGui::Text("Size");
	ImGui::SliderFloat("scale", &img1_size, 0.0f, 2000.0f);
	ImGui::Spacing();

	//Select operations
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Text("List of avaliable image operations");
	ImGui::Combo("operation", &currentOperation, operations, IM_ARRAYSIZE(operations), TOTAL_OPERATIONS);

	//Select Images (Drop down menu)
	ImGui::Spacing();
	ImGui::Text("Import image(s) (just filename, exclude .ppm): ");
	ImGui::InputText("Image #1", file1, 256);
	if (currentOperation < 4)
	{
		ImGui::Text("Import second image: ");
		ImGui::InputText("Image #2", file2, 256);
	}
	else if (currentOperation >= 4)
	{
		ImGui::InputFloat("Intensity Value", &intensity);
		if (currentOperation == 5)
			ImGui::InputFloat("Gamma Value", &gamma);
	}

	//Operate button
	operateImg = ImGui::Button("Operate");

	//Display result image
	if (hasResult)
		displayResult = ImGui::Button("Display & Write Result");

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / 
		ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void Screen::OperateImg()
{
	//Arthmetic operations - uses 2 images
	if (currentOperation < 4)
	{
		outImage = ArithmeticOperation(&img1, &img2, currentOperation);
	}
}

void Screen::CleanUp()
{
	delete file1;
	file1 = nullptr;
	delete file2;
	file2 = nullptr;
	
	delete shader;
}

void Screen::UpdateWindowDimensions(int w, int h)
{
	windowWidth = w;
	windowHeight = h;
}
