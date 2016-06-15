package
{
	import flash.geom.Point;
	import net.flashpunk.Entity;
	import net.flashpunk.graphics.Image;
	public class Agent extends Entity
	{
		[Embed(source = 'assets/fish.png')] private const PLAYER:Class;
		public function Agent()
		{
				graphic = new Image(PLAYER);
		}
		
		public var velocity:Point;
		
	}
}