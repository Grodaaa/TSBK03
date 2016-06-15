    	for(bonesnum = 1; bonesnum < kMaxBones; bonesnum++)
    	{
    		mat4 currOrgT = T(g_bones[bonesnum].pos.x, g_bones[bonesnum].pos.y, g_bones[bonesnum].pos.z);
    		mat4 preOrgT = T(g_bones[bonesnum - 1].pos.x, g_bones[bonesnum - 1].pos.y, g_bones[bonesnum - 1].pos.z);

    		mat4 currResT = T(g_bonesRes[bonesnum].pos.x, g_bonesRes[bonesnum].pos.y, g_bonesRes[bonesnum].pos.z);
    		mat4 preResT = T(g_bonesRes[bonesnum - 1].pos.x, g_bonesRes[bonesnum - 1].pos.y, g_bonesRes[bonesnum - 1].pos.z);

    		mat4 currOrgM = Mult(currOrgT, g_bones[bonesnum].rot);
    		mat4 preOrgM = Mult(preOrgT, g_bones[bonesnum - 1].rot);

    		mat4 currResM = Mult(currResT, g_bonesRes[bonesnum].rot);
    		mat4 preResM = Mult(preResT, g_bonesRes[bonesnum - 1].rot);

    		mat4 currInvM = InvertMat4(currOrgM);
    		mat4 preInvM = InvertMat4(currOrgM);

    		mat4 orgM = Mult(preOrgM, currOrgM);
    		mat4 resM = Mult(preResM, currResM);
    		mat4 invM = Mult(currInvM, preInvM);

    		vec3 resV = MultVec3(orgM, g_vertsOrg[row][corner]);

			//g_vertsRes[row][corner] = ScalarMult( MultVec3( Mult(resM, invM), resV), g_boneWeights[row][corner][bonesnum]);
			sum = VectorAdd(ScalarMult( MultVec3( Mult(resM, invM), resV), g_boneWeights[row][corner][bonesnum]), sum);
    	}
