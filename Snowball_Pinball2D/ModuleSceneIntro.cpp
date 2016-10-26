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

	circle = background = foreground = kickers = objects = NULL;



	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	ball_available = true;
	lose = false;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/ball.png"); 
	//box = App->textures->Load("pinball/crate.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	background = App->textures->Load("pinball/background.png");
	foreground = App->textures->Load("pinball/foreground.png");

	kickers = App->textures->Load("pinball/kickers.png");

	App->audio->PlayMusic("pinball/Pixeljam - Snowball Theme.ogg");




	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, 670, SCREEN_WIDTH, 10);
	teleporter = App->physics->CreateRectangleSensor(147, 200, 15, 5);
	accelerator_spiral = App->physics->CreateRectangleSensor(73, 170, 20, 5);
	accelerator_right = App->physics->CreateRectangleSensor(455, 320, 5, 20);

	accelerator_left = App->physics->CreateRectangleSensor(350, 323, 5, 20);

	accelerator_right_bot = App->physics->CreateRectangleSensor(695, 1305, 5, 20);
	accelerator_left_bot = App->physics->CreateRectangleSensor(180, 1337, 5, 20);
	game_over_sensor = App->physics->CreateRectangleSensor(400, 1430, 90, 10);

	accelerator_left = App->physics->CreateRectangleSensor(350, 320, 5, 20);

	bot_right_spring = App->physics->CreateRectangleSensor(780, 1150, 5, 20);
	bot_left_spring = App->physics->CreateRectangleSensor(75, 950, 15, 5);
	mid_spring = App->physics->CreateRectangleSensor(540, 710, 5, 20);


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


	//kickers
	
	kicker_l = App->physics->CreatePolygon(340, 1360, leftKicker, 8);
	PhysBody* C = App->physics->CreateCircle(340, 1360, 7, b2BodyType::b2_staticBody);
	rJoint_left = App->physics->createRevoluteJoint(kicker_l, C, 0, 0, -5, -15, 15);

	kicker_r = App->physics->CreatePolygon(465, 1360, rightKicker, 8);
	PhysBody* B = App->physics->CreateCircle(465, 1360, 7, b2BodyType::b2_staticBody);
	rJoint_right = App->physics->createRevoluteJoint(kicker_r, B, 0, 0, -180, 165, 200);

	kicker_l_2 = App->physics->CreatePolygon(350, 595, leftKicker2, 8);
	PhysBody* C2 = App->physics->CreateCircle(350, 595, 7, b2BodyType::b2_staticBody);
	rJoint_left2 = App->physics->createRevoluteJoint(kicker_l_2, C2, 0, 0, -5, -15, 15);

	kicker_r_2 = App->physics->CreatePolygon(480, 595, rightKicker2, 8);
	PhysBody* B2 = App->physics->CreateCircle(480, 595, 7, b2BodyType::b2_staticBody);
	rJoint_right2 = App->physics->createRevoluteJoint(kicker_r_2, B2, 0, 0, -180, 165, 200);

	kicker_l_3 = App->physics->CreatePolygon(610, 400, leftKicker3, 8);
	PhysBody* C3 = App->physics->CreateCircle(610, 400, 7, b2BodyType::b2_staticBody);
	rJoint_left3 = App->physics->createRevoluteJoint(kicker_l_3, C3, 0, 0, -5, -15, 15);

	kicker_r_3 = App->physics->CreatePolygon(207, 945, rightKicker3, 8);
	PhysBody* B3 = App->physics->CreateCircle(207, 945, 7, b2BodyType::b2_staticBody);
	rJoint_right3 = App->physics->createRevoluteJoint(kicker_r_3, B3, 0, 0, -140, 165, 200);


	boxes.add(App->physics->CreateRectangle(770, 370, 12, 12, b2BodyType::b2_staticBody));
	boxes.getLast()->data->listener = this;


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	/*
	App->textures->Unload(foreground);
	App->textures->Unload(background);
	App->textures->Unload(box);
	App->textures->Unload(circle);
	*/
	
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	
	
		App->renderer->Blit(background, 0, 0, NULL, 1.0f, NULL);
	
	

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		if (ball == NULL)
		{
			ball = App->physics->CreateCircle(770, 340, 8, b2BodyType::b2_dynamicBody);
			ball->listener = this;
		}
		
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		rJoint_right->SetMotorSpeed(-30);
		rJoint_right2->SetMotorSpeed(-30);
		rJoint_right3->SetMotorSpeed(-30);
		
	}
	else
	{
		rJoint_right->SetMotorSpeed(30);
		rJoint_right2->SetMotorSpeed(30);
		rJoint_right3->SetMotorSpeed(30);
		
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		rJoint_left->SetMotorSpeed(30);
		rJoint_left2->SetMotorSpeed(30);
		rJoint_left3->SetMotorSpeed(30);

	}
	else
	{
		rJoint_left->SetMotorSpeed(-30);
		rJoint_left2->SetMotorSpeed(-30);
		rJoint_left3->SetMotorSpeed(-30);

	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && ball_available == true)
	{
		if (ball != NULL)
		{
			ball->body->ApplyForce(b2Vec2(0, -80), b2Vec2(770, 350), true);
		}
		
		ball_available = false;

	}

	

	

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	

	fVector normal(0.0f, 0.0f);
	int x, y;
	if (ball != NULL)
	{
		ball->GetPosition(x, y);
		ball->body->SetTransform(b2Vec2(ball->body->GetPosition().x, ball->body->GetPosition().y), 90);

		

		App->renderer->Blit(circle, x - 2, y, NULL, 1.0f, ball->GetRotation());
	}
	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		

		

		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	/*while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		
		c = c->next;
	}*/

	App->renderer->Blit(foreground, 0, 0, NULL, 1.0f, NULL);

	//springs
	
	
	
	

	//left
	SDL_Rect section;
	section.x = 10;
	section.y = 0;
	section.h = 40;
	section.w = 37;

	App->renderer->Blit(kickers, 340, 1355, &section, 1.0f, (-rJoint_left->GetJointAngle() * RADTODEG) + 320, 0, 0);
	
	App->renderer->Blit(kickers, 350, 590, &section, 1.0f, (-rJoint_left2->GetJointAngle() * RADTODEG) + 320, 0, 0);
	

	App->renderer->Blit(kickers, 610, 390, &section, 1.0f, (-rJoint_left3->GetJointAngle() * RADTODEG) + 320, 0, 0);
	//right
	SDL_Rect section1;
	section1.x = 63;
	section1.y = 0;
	section1.h = 40;
	section1.w = 37;
	App->renderer->Blit(kickers, 435, 1340, &section1, 1.0f, (-rJoint_right->GetJointAngle() * RADTODEG) + 230, 20, 10);

	
	App->renderer->Blit(kickers, 443, 580, &section1, 1.0f, (-rJoint_right2->GetJointAngle() * RADTODEG) + 230, 20, 10);

	
	//App->renderer->Blit(kickers, 207, 945, &section1, 1.0f, (-rJoint_right3->GetJointAngle() * RADTODEG) + 210, 0, 0);


	if (lose == true)
	{
		App->physics->deleteBall(ball->body);
		ball = nullptr;
		lose = false;
		sensed = false;
		App->renderer->camera.x = App->renderer->camera.y = 0;

		if (ball == NULL)
		{
			ball = App->physics->CreateCircle(770, 340, 8, b2BodyType::b2_dynamicBody);
			ball->listener = this;
		}
	}


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	//App->audio->PlayFx(bonus_fx);



	if(bodyB == sensor)

	{
		if (sensed != true)
		{
			sensed = true;
			App->renderer->camera.x = 0;
			for(int i = 0; i > -620; i -= 20)
			
			App->renderer->camera.y -= 20;
		}
		else
		{
			sensed = false;
			App->renderer->camera.x = App->renderer->camera.y = 0;
		}
	}
	else if (bodyB == game_over_sensor)
	{
		
		lose = true;
		ball_available = true;
		
		
	}else if(bodyB == accelerator_left || bodyB == accelerator_spiral  || bodyB == accelerator_right_bot)
	{
		bodyA->body->SetLinearVelocity(b2Vec2(-10, 20));
		
		
	
	}
	else if (bodyB == accelerator_right || bodyB == accelerator_left_bot)
	{
		bodyA->body->SetLinearVelocity(b2Vec2(10, 10));

	}
	else if (bodyB == bot_right_spring)
	{
		
		bodyA->body->ApplyForce(b2Vec2(-50, 0), b2Vec2(782, 1152), true);
	}
	else if (bodyB == bot_left_spring)
	{
		
		bodyA->body->ApplyForce(b2Vec2(0, -50), b2Vec2(77, 950), true);
	}
	else if (bodyB == mid_spring )
	{
		
		bodyA->body->ApplyForce(b2Vec2(50, 0), b2Vec2(542, 712), true);
	}
	else if (bodyB == teleporter)
	{
		lose = true;
		ball_available = true;
	}
	
}
