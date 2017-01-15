/*
* Copyright (c) 2008-2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef SHAPE_EDITING_H
#define SHAPE_EDITING_H
#include <vector>

class Gun {
public:
	b2Body* m_body;

	Gun(b2World* world) {
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		m_body = world->CreateBody(&bodyDef);

		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(0.5f, 0.5f);
		m_body->CreateFixture(&polygonShape, 1);//shape, density

		m_body->SetUserData(this);
	}

	~Gun() {
		m_body->GetWorld()->DestroyBody(m_body);
	}
};

class ShapeEditing : public Test
{
public:

	enum _moveState {
		MS_STOP,
		MS_LEFT,
		MS_RIGHT,
		MS_UP,
		MS_DOWN,
	};


	b2Body* playerBody;
	_moveState moveState;
	std::vector<Gun*> m_guns;
	b2RevoluteJoint *flJoint, *frJoint;


	ShapeEditing()
	{
		/*{
			b2BodyDef bd;
			b2Body* ground = m_world->CreateBody(&bd);

			b2EdgeShape shape;
			shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
			ground->CreateFixture(&shape, 0.0f);
		}

		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.position.Set(0.0f, 10.0f);
		m_body = m_world->CreateBody(&bd);

		b2PolygonShape shape;
		shape.SetAsBox(4.0f, 4.0f, b2Vec2(0.0f, 0.0f), 0.0f);
		m_fixture1 = m_body->CreateFixture(&shape, 10.0f);

		m_fixture2 = NULL;

		m_sensor = false;*/

		//body definition
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_dynamicBody;

		//shape definition
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(2, 2); //a 2x2 rectangle

		//fixture definition
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &polygonShape;
		myFixtureDef.density = 1;

		//create dynamic body
		myBodyDef.position.Set(0, 10);
		playerBody = m_world->CreateBody(&myBodyDef);
		playerBody->CreateFixture(&myFixtureDef);

		myBodyDef.type = b2_staticBody;
		myBodyDef.position.Set(0, 0);
		b2Body* staticBody = m_world->CreateBody(&myBodyDef);

		//add four walls to the static body
		polygonShape.SetAsBox(20, 1, b2Vec2(0, 0), 0);//ground
		staticBody->CreateFixture(&myFixtureDef);
		polygonShape.SetAsBox(20, 1, b2Vec2(0, 40), 0);//ceiling
		staticBody->CreateFixture(&myFixtureDef);
		polygonShape.SetAsBox(1, 20, b2Vec2(-20, 20), 0);//left wall
		staticBody->CreateFixture(&myFixtureDef);
		polygonShape.SetAsBox(1, 20, b2Vec2(20, 20), 0);//right wall
		staticBody->CreateFixture(&myFixtureDef);



		b2RevoluteJointDef jointDef;
		jointDef.bodyA = playerBody;
		jointDef.enableLimit = true;
		jointDef.lowerAngle = 0;//with both these at zero...
		jointDef.upperAngle = 0;//...the joint will not move
		jointDef.localAnchorB.SetZero();//joint anchor in tire is always center

		Gun* gun = new Gun(m_world);
		jointDef.bodyB = gun->m_body;
		jointDef.localAnchorA.Set(0, 0.0f);
		flJoint = (b2RevoluteJoint*)m_world->CreateJoint(&jointDef);
		m_guns.push_back(gun);










		moveState = MS_STOP;



	}

	void Keyboard(unsigned char key)
	{
		switch (key)
		{
		/*case 'c':
			if (m_fixture2 == NULL)
			{
				b2CircleShape shape;
				shape.m_radius = 3.0f;
				shape.m_p.Set(0.5f, -4.0f);
				m_fixture2 = m_body->CreateFixture(&shape, 10.0f);
				m_body->SetAwake(true);
			}
			break;

		case 'd':
			if (m_fixture2 != NULL)
			{
				m_body->DestroyFixture(m_fixture2);
				m_fixture2 = NULL;
				m_body->SetAwake(true);
			}
			break;

		case 's':
			if (m_fixture2 != NULL)
			{
				m_sensor = !m_sensor;
				m_fixture2->SetSensor(m_sensor);
			}
			break;*/

		case ',':
			if (m_bullet != NULL)
			{
				m_world->DestroyBody(m_bullet);
				m_bullet = NULL;
			}

			{
				b2CircleShape shape;
				shape.m_radius = 0.25f;

				b2FixtureDef fd;
				fd.shape = &shape;
				fd.density = 20.0f;
				fd.restitution = 0.05f;

				
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.bullet = true;
				float x = playerBody->GetPosition().x;
				float y = playerBody->GetPosition().y;
				
				bd.position.Set(x, y);

				m_bullet = m_world->CreateBody(&bd);
				m_bullet->CreateFixture(&fd);

				if (moveState == MS_UP)
				{
					m_bullet->SetLinearVelocity(b2Vec2(0.0f, 400.0f));
				}

				else if (moveState == MS_DOWN)
				{
					m_bullet->SetLinearVelocity(b2Vec2(0.0f, -400.0f));
				}

				else if (moveState == MS_LEFT)
				{
					m_bullet->SetLinearVelocity(b2Vec2(-400.0f, 0.0f));
				}

				else if (moveState == MS_RIGHT)
				{
					m_bullet->SetLinearVelocity(b2Vec2(400.0f, 0.0f));
				}


			}
			break;
		case 'e': //stop
			moveState = MS_STOP;
			break;
		case 'a': //move left
			moveState = MS_LEFT;
			break;
		case 'd': //move right
			moveState = MS_RIGHT;
			break;
		case 's': //move right
			moveState = MS_DOWN;
			//m_bullet->SetLinearVelocity(b2Vec2(0.0f, -400.0f));
			break;
		case 'w': //move right
			moveState = MS_UP;
			//m_bullet->SetLinearVelocity(b2Vec2(0.0f, 400.0f));
			break;
		default:
			//run default behaviour
			Test::Keyboard(key);



		}
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);
		/*m_debugDraw.DrawString(5, m_textLine, "Press: (c) create a shape, (d) destroy a shape.");
		m_textLine += DRAW_STRING_NEW_LINE;
		m_debugDraw.DrawString(5, m_textLine, "sensor = %d", m_sensor);
		m_textLine += DRAW_STRING_NEW_LINE;*/

		b2Vec2 vel = playerBody->GetLinearVelocity();
		switch (moveState)
		{
		case MS_LEFT:  vel.x = -5; break;
		case MS_STOP:  vel.x = 0; break;
		case MS_RIGHT: vel.x = 5; break;
		case MS_UP: vel.y = 5; break;
		case MS_DOWN: vel.y = -5; break;

		}
		playerBody->SetLinearVelocity(vel);

		
	}

	static Test* Create()
	{
		return new ShapeEditing;
	}

	b2Body* m_bullet;
	b2Fixture* m_fixture1;
	b2Fixture* m_fixture2;
	bool m_sensor;
};




#endif
