#include "raylib.h"
#include <stdio.h>

#define MAX_LIGHTS 4

typedef enum {
    LIGHT_DIRECTIONAL = 0,
    LIGHT_POINT,
    LIGHT_SPOT
} LightType;

typedef struct {
    int type;
    int enabled;
    Vector3 position;
    Vector3 target;
    Color color;
    float intensity;
} Light;

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    InitWindow(screenWidth, screenHeight, "Old Rusty Car Demo");
    
    Camera camera = { 0 };
    camera.position = (Vector3){ 2.0f, 2.0f, 6.0f };
    camera.target = (Vector3){ 0.0f, 0.5f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    // Load car model (using relative path)
    Model car = LoadModel("models/old_car_new.glb");
    if (car.meshCount == 0) {
        printf("ERROR: Could not load car model!\n");
        printf("Make sure the file exists at: models/old_car_new.glb\n");
        return 1;
    }
    
    // Load textures
    Texture2D albedo = LoadTexture("resources/old_car_d.png");
    Texture2D mra = LoadTexture("resources/old_car_mra.png");
    Texture2D normal = LoadTexture("resources/old_car_n.png");
    Texture2D emissive = LoadTexture("resources/old_car_e.png");
    
    // Assign textures to material
    car.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = albedo;
    car.materials[0].maps[MATERIAL_MAP_NORMAL].texture = normal;
    
    // Create floor
    Mesh floorMesh = GenMeshPlane(10.0f, 10.0f, 10, 10);
    Model floor = LoadModelFromMesh(floorMesh);
    floor.materials[0].maps[MATERIAL_MAP_ALBEDO].color = DARKGRAY;
    
    // Create lights
    Light lights[MAX_LIGHTS] = { 0 };
    lights[0] = (Light){ LIGHT_POINT, 1, (Vector3){-1.0f, 1.0f, -2.0f}, (Vector3){0,0,0}, YELLOW, 4.0f };
    lights[1] = (Light){ LIGHT_POINT, 1, (Vector3){ 2.0f, 1.0f,  1.0f}, (Vector3){0,0,0}, GREEN, 3.3f };
    lights[2] = (Light){ LIGHT_POINT, 1, (Vector3){-2.0f, 1.0f,  1.0f}, (Vector3){0,0,0}, RED, 8.3f };
    lights[3] = (Light){ LIGHT_POINT, 1, (Vector3){ 1.0f, 1.0f, -2.0f}, (Vector3){0,0,0}, BLUE, 2.0f };
    
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);
        
        // Toggle lights with number keys
        if (IsKeyPressed(KEY_ONE)) lights[0].enabled = !lights[0].enabled;
        if (IsKeyPressed(KEY_TWO)) lights[1].enabled = !lights[1].enabled;
        if (IsKeyPressed(KEY_THREE)) lights[2].enabled = !lights[2].enabled;
        if (IsKeyPressed(KEY_FOUR)) lights[3].enabled = !lights[3].enabled;
        
        BeginDrawing();
            ClearBackground(BLACK);
            
            BeginMode3D(camera);
                // Draw floor
                DrawModel(floor, (Vector3){0, -0.5f, 0}, 1.0f, WHITE);
                
                // Draw car
                DrawModel(car, (Vector3){0, 0, 0}, 0.25f, WHITE);
                
                // Draw light spheres
                for (int i = 0; i < MAX_LIGHTS; i++) {
                    if (lights[i].enabled) {
                        DrawSphereEx(lights[i].position, 0.15f, 8, 8, lights[i].color);
                    } else {
                        DrawSphereWires(lights[i].position, 0.15f, 8, 8, ColorAlpha(lights[i].color, 0.3f));
                    }
                }
                
                DrawGrid(10, 1.0f);
            EndMode3D();
            
            DrawText("Press 1-4 to toggle lights", 10, 40, 20, LIGHTGRAY);
            DrawFPS(10, 10);
        EndDrawing();
    }
    
    // Cleanup
    UnloadTexture(albedo);
    UnloadTexture(mra);
    UnloadTexture(normal);
    UnloadTexture(emissive);
    UnloadModel(car);
    UnloadModel(floor);
    
    CloseWindow();
    return 0;
}