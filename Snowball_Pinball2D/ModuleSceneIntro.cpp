#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = background = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/ball.png"); 
	box = App->textures->Load("pinball/crate.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	background = App->textures->Load("pinball/background.png");
	foreground = App->textures->Load("pinball/foreground.png");

	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	//outer map chains
	Chains.add(App->physics->CreateChain(0, 0, map, 472, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, map2, 24, b2BodyType::b2_staticBody));
	//bouncers
	Chains.add(App->physics->CreateChain(0, 0, left_bouncer, 10, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, right_bouncer, 10, b2BodyType::b2_staticBody));
	//right part
	Chains.add(App->physics->CreateChain(0, 0, bottom1, 72, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, bottom_little, 26, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, little_top_cave, 26, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, very_little, 12, b2BodyType::b2_staticBody));
	//left part
	Chains.add(App->physics->CreateChain(0, 0, left_top, 82, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, left_tunnel, 40, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, left_bot, 50, b2BodyType::b2_staticBody));

	//middle part
	Chains.add(App->physics->CreateChain(0, 0, snow_pile, 32, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, right_corridor, 20, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, left_corridor, 18, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, middle_top, 58, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, middle_little, 22, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, middle_little2, 18, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, middle_bot, 66, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, middle_bot2, 30, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, middle_bot3, 48, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, middle_bot4, 16, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, middle_mid, 90, b2BodyType::b2_staticBody));
	Chains.add(App->physics->CreateChain(0, 0, middle_mid2, 34, b2BodyType::b2_staticBody));
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	
	App->textures->Unload(foreground);
	App->textures->Unload(background);
	App->textures->Unload(box);
	App->textures->Unload(circle);
	
	
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	/*if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}*/
	
		App->renderer->Blit(background, 0, 0, NULL, 1.0f, NULL);
	
	

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 8));
		circles.getLast()->data->listener = this;
	}
	

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		//if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	App->renderer->Blit(foreground, 0, 0, NULL, 1.0f, NULL);
	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	//App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
