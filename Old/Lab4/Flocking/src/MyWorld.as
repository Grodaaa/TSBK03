package
{
	import flash.geom.Point;
	import net.flashpunk.World;
	import net.flashpunk.Entity;
	import net.flashpunk.FP;
	import net.flashpunk.graphics.Image;
	import punk.ui.PunkButton;
	import punk.ui.PunkLabel;
	//import punk.ui.PunkSlider;
	import punk.ui.*;
	public class MyWorld extends World

	{
		
		public var agentArray:Array;
		public var alignmentLabel:PunkLabel;
		public var cohesionLabel:PunkLabel;
		public var separationLabel:PunkLabel;
		public var alignmentWeight = 0.1;
		public var cohesionWeight = 0.1;
		public var separationWeight = 0.1;
		public var alignmentSlider:PunkSlider;
		public var cohesionSlider:PunkSlider;
		public var separationSlider:PunkSlider;
		public var resetButton:PunkButton;

		public function MyWorld()
		{
			super();
			
			loadLevel();
			
			alignmentLabel = new PunkLabel("Alignment: ", 25, 530);
			cohesionLabel = new PunkLabel("Cohesion: ", 25, 550);
			separationLabel = new PunkLabel("Separation: ", 25, 570);
			alignmentSlider = new PunkSlider(250, 530, 500, 16, alignmentChanged, 0, 1, alignmentWeight, false);
			alignmentSlider.setCallback(alignmentChanged);
			add(alignmentSlider);
			cohesionSlider = new PunkSlider(250, 550, 500, 16, cohesionChanged, 0, 1, cohesionWeight, false);
			cohesionSlider.setCallback(cohesionChanged);
			add(cohesionSlider);
			separationSlider = new PunkSlider(250, 570, 500, 16, separationChanged, 0, 1, separationWeight, false);
			separationSlider.setCallback(separationChanged);
			resetButton = new PunkButton(10, 10, 100, 25, "Reset");
			add(resetButton);
			resetButton.setCallbacks(null, loadLevel);
			
			add(separationSlider);
			add(alignmentLabel);
			add(cohesionLabel);
			add(separationLabel);
			//add(resetButton);
		}
		public function loadLevel():void 
		{
			for each (var e:Agent in agentArray)
			{
				remove(agent);
			}
			agentArray = new Array;
			for (var i:Number = 0; i < 20; i++)
			{
				var agent:Agent = new Agent;
				agent.x = Math.random() * FP.width;
				agent.y = Math.random() * FP.height;
				agent.velocity = new Point();
				agent.velocity.x = (Math.random() * 10) - 5;
				agent.velocity.y = (Math.random() * 10) - 5;
				agentArray.push(agent)
				add(agent);
			}
		}
		
		public function alignmentChanged(x):void 
		{
			alignmentWeight = alignmentSlider.value;
		}
		public function cohesionChanged(x):void 
		{
			cohesionWeight = cohesionSlider.value;
		}
		public function separationChanged(x):void 
		{
			separationWeight = separationSlider.value;
		}
		
		public function computeAlignment(myAgent:Agent):Point 
		{
			var v:Point = new Point();
			var neighborCount = 0;
				for each (var agent:Agent in agentArray) 
				{
					if (agent != myAgent)
					{
						if (myAgent.distanceFrom(agent) < 300)
						{
							v.x += agent.velocity.x;
							v.y += agent.velocity.y;
							neighborCount++;
						}
						
					}

				}
			if (neighborCount == 0)
				return v;
			v.x /= neighborCount;
			v.y /= neighborCount;
			v.normalize(1);
			return v;
		}
		
		public function computeCohesion(myAgent:Agent):Point 
		{
			var v:Point = new Point();
			var neighborCount = 0;
			for each (var agent:Agent in agentArray) 
			{
				if (agent != myAgent)
				{
					if (myAgent.distanceFrom(agent) < 300)
					{
						v.x += agent.x;
						v.y += agent.y;
						neighborCount++;
					}
				}
	
			}
			if (neighborCount == 0)
				return v;
			v.x /= neighborCount;
			v.y /= neighborCount;
			v = new Point(v.x - myAgent.x, v.y - myAgent.y);
			v.normalize(1);
			return v;
		}
		
		public function computeSeparation(myAgent:Agent):Point 
		{
			var v:Point = new Point();
			var neighborCount = 0;
			for each (var agent:Agent in agentArray) 
			{
				if (agent != myAgent)
				{
					if (myAgent.distanceFrom(agent) < 300)
					{
						v.x += agent.x - myAgent.x;
						v.y += agent.y - myAgent.y
						neighborCount++;
					}
				}
	
			}
			if (neighborCount == 0)
				return v;
			v.x /= neighborCount;
			v.y /= neighborCount;
			v.x *= -1;
			v.y *= -1;
			v.normalize(1);
			
			return v;
		}
		
		public function computeWallAvoidance(agent:Agent):Point 
		{
			var v:Point = new Point();
			if (agent.left < 40)
			{
				v.x = 10;
			}
			if (agent.right > FP.width-40)
			{
				v.x = -10;
			}
			if (agent.top < 40)
			{
				v.y = 10;
			}
			if (agent.bottom > FP.height-40)
			{
				v.y = -10;
			}
			return v;
		}
		
		
		override public function update():void 
		{
			super.update();
			for each (var agent:Agent in agentArray)
			{
				var alignment = computeAlignment(agent);
				var cohesion = computeCohesion(agent);
				var separation = computeSeparation(agent);
				var wallAvoidance = computeWallAvoidance(agent);
		    	agent.velocity.x += alignment.x * alignmentWeight + cohesion.x * cohesionWeight + separation.x * separationWeight;
				agent.velocity.y += alignment.y * alignmentWeight + cohesion.y * cohesionWeight + separation.y * separationWeight;
				
				agent.velocity.x += wallAvoidance.x * 0.1;
				agent.velocity.y += wallAvoidance.y * 0.1;
				agent.velocity.normalize(5);
				Image(agent.graphic).angle = Math.atan2(-agent.velocity.y, agent.velocity.x) * 180 / Math.PI;
				
				agent.x += agent.velocity.x;
				agent.y += agent.velocity.y;
			}
			
			alignmentLabel.text = "Alignment: " + alignmentWeight;
			cohesionLabel.text = "Cohesion: " + cohesionWeight;
			separationLabel.text = "Separation: " + separationWeight;
			
			
		}
		
	}
}