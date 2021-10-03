// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

const char* GAME_WINDOW_TITLE = "Ludum Dare 49";

const char* hint1_titles[] = {"1A", "2L", "3C", "4O", "5H", "6O", "7L"};
const ImVec4 hint1_colors[] = {
    ImVec4(1.0f,0.0f,0.0f,1.0f),
    ImVec4(1.0f,0.647f,0.0f,1.0f),
    ImVec4(1.0f,1.0f,0.0f,1.0f),
    ImVec4(0.0f,0.501f,0.0f,1.0f),
    ImVec4(0.0f,0.0f,1.0f,1.0f),
    ImVec4(0.294f,0.0f,0.509f,1.0f),
    ImVec4(0.933f,0.509f,0.933f,1.0f),
};
const char* hint1[] = {
    "A",
    "L",
    "C",
    "O",
    "H",
    "O",
    "L"
};


// Main code
int main(int, char**)
{
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow(GAME_WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // State
    char open_filename[24];
    char input_password[24];
    bool button_null[50];
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("README.txt");

            ImGui::TextWrapped(
"IN EVENT OF GAME DISASTER:\n\
===========================\n\n\
The game is quite unstable ever since we fired all those people.\n\
Usually there are a couple of things to keep in mind: \
All core components should be enabled. \
Experimental features should be disabled.\n\
Fortunately, a bunch of smart people who came before you left notes on what they found lying around \
(when they were desperately trying to make this work).\n\
");

            /* ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state */
            /* ImGui::Checkbox("Another Window", &show_another_window); */

            /* ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f */
            /* ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color */

            /* if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated) */
            /*     counter++; */
            /* ImGui::SameLine(); */
            /* ImGui::Text("counter = %d", counter); */

            /* ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); */
            ImGui::End();
        }
        
        {
            ImGui::Begin("Game", NULL, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("Core Components");
            ImGui::Checkbox("Video", &button_null[0]);
            ImGui::Checkbox("Audio", &button_null[1]);
            ImGui::Checkbox("Characters", &button_null[2]);
            ImGui::Checkbox("Wild Animals", &button_null[3]);
            ImGui::Checkbox("Stability Module", &button_null[6]);
            ImGui::Checkbox("Trees", &button_null[4]);
            ImGui::Checkbox("Skybox", &button_null[5]);
            ImGui::Checkbox("Enemies", &button_null[11]);
            ImGui::Checkbox("Multiplayer", &button_null[4]);
            ImGui::Checkbox("Buildings", &button_null[12]);
            ImGui::Text("Experimental Features");
            ImGui::Checkbox("NPC Marriage", &button_null[9]);
            ImGui::Checkbox("Fishing minigame", &button_null[8]);
            ImGui::Checkbox("Vim keybindings", &button_null[10]);
            ImGui::Checkbox("Japanese support", &button_null[13]);
            ImGui::End();
        }

        {
            ImGui::Begin("Open File", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
            ImGui::InputText("Filename", open_filename, IM_ARRAYSIZE(open_filename), 0);
            ImGui::End();
        }

        if(!strcmp(open_filename, "EMAILS.txt")) {
            ImGui::Begin("EMAILS.txt");
            ImGui::TextWrapped("From: Ani    (today)\n    I've reviewed your proposal to add NPC gay marriage as an experimental feature, but we're not very happy about it. You should reconsider... if you don't want to face the consequences.");
            ImGui::TextWrapped("From: B    (today)\n    I watched that anime (serial experiments lain) you were telling me about, we should meet to talk about it.");
            ImGui::TextWrapped("From: A friend    (16/7/2004)\n    I heard you fired everyone from your game. It doesn't sound like you. I hope you're doing well. XoXo");
            ImGui::TextColored(hint1_colors[0], "From: ????    (11/7/2004)\n    TONIGHT. EVERYONE'S GONE TONIGHT.");
            ImGui::TextWrapped("To: Ani    (24/6/2004)\n    Feature branch (attached): NPC gay marriage.");
            ImGui::End();
        }
        
        {
            ImGui::Begin("*** Classified ***", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
            ImGui::InputText("Password", input_password, IM_ARRAYSIZE(input_password), ImGuiInputTextFlags_Password);
            ImGui::End();
        }

        if(!strcmp(input_password, "lain")) {
            ImGui::Begin("Dear Diary");
            ImGui::TextWrapped("(10/7/2004)\nI f*cked up. I'm sorry everyone. I didn't mean for this to end like this.\n\
The game is over -- its crash is a façade. There's nothing I can do anymore -- they took over. The game got big, so they claimed it back...\n\
This is no longer my game... our game...\n\
Dear friend, if you are reading this, I don't want you to worry.\n\
Unknown to you all, I'm leaving as well. I hope you can all start over.\n\
");
            ImGui::End();
        }

        {
            ImGui::Begin("Restart", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
            ImGui::Button("Restart");
            ImGui::End();
        }

        {
            ImGui::Begin("Unknown", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
            ImGui::Button("Unknown");
            ImGui::End();
        }

        {
            ImGui::Begin("Notes");
            if(ImGui::TreeNode("tigre_79")) {
                ImGui::TextWrapped("\
Nsgre gur tnzr penfurq V ybbxrq nebhaq... Gur qrirybcre vf va fbzr funql qrnyf... \
Fbzr vasbezngvba frrzf gb or zvffvat sebz gur RZNVYF.gkg svyr.\
");
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("#4")) {
                ImGui::TextWrapped("I overheard the developer's a real f*cking weeb.");
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("knorr")) {
                ImGui::TextWrapped("Der Lehrer sagte mir, dass er nicht mehrleben wollte, nachdem er entdeckt, was seine Student tat.");
                ImGui::TreePop();
            }
            ImGui::End();
        }

        /* for (int i=0; i<7; i++) { */
        /*     ImGui::Begin(hint1_titles[i], NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar); */
        /*     ImGui::TextColored(hint1_colors[i], "%s", hint1[i]); */
        /*     ImGui::End(); */
        /* } */

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
