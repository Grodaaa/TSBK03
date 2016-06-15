SpritePtr temp, temp2, update;
	float maxDist = 250, maxSpeed = 2.0, alignmentWeight = 0.1, cohesionWeight = 0.1;

	temp = gSpriteRoot;
	do
	{
		int counter = 0;
		/*temp->speedDiff.h = temp->speedDiff.v = 0.0;
		temp->avgPos.h = temp->avgPos.v = 0.0;
		temp->avoVec.h = temp->avoVec.v = 0.0;*/

		temp2 = gSpriteRoot;
		do
		{
			if(temp != temp2)
			{	
				float dist = sqrt((temp2->position.h*temp2->position.h) - (temp->position.h*temp->position.h) + 
								  (temp2->position.v*temp2->position.v) - (temp->position.v*temp->position.v));

				if(dist < maxDist)
				{
					//For alignment
					temp->speedDiff.h += temp2->speed.h;
					temp->speedDiff.v += temp2->speed.v;
					//For cohesion
					temp->avgPos.h += temp2->position.h;
					temp->avgPos.v += temp2->position.v;
					//For avoidance
					temp->avoVec.h += temp2->position.h - temp->position.h;
					temp->avoVec.v += temp2->position.v - temp->position.v;

					counter++;
				}
			}

			temp2 = temp2->next;
		} while(temp2 != NULL);

		if(counter > 0)
		{
			temp->speedDiff.h /= counter;
			temp->speedDiff.h /= counter;

			temp->avgPos.h /= counter;
			temp->avgPos.v /= counter;

			temp->avoVec.h /= counter;
			temp->avoVec.v /= counter;

			temp->speed.h += temp->speedDiff.h * alignmentWeight + (temp->avgPos.h - temp->position.h) * cohesionWeight + temp->avoVec.h;
			temp->speed.v += temp->speedDiff.v * alignmentWeight + (temp->avgPos.v - temp->position.v) * cohesionWeight + temp->avoVec.v;
		}

		float length = sqrt((temp->speed.h * temp->speed.h) + (temp->speed.v * temp->speed.v));

		if(length > maxSpeed)
		{
			temp->speed.h = (temp->speed.h/length)*maxSpeed;
			temp->speed.v = (temp->speed.v/length)*maxSpeed;
		}

		temp->position.h += temp->speed.h; 
		temp->position.v += temp->speed.v;
		
		temp = temp->next;
	} while(temp != NULL);