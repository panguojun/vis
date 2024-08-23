/**					持续动画可以看作一种旋转
*					使用旋转三角函数自动实现
*/
namespace spin
{
	int frame = 0;
	real sin(real ang, real speed)
	{
		real dang = frame * speed;
		return ::sin(ang + dang);
	}
	quat QX(real speed)
	{
		real deg = speed;
		return quat(deg * PI / 180, vec3::UX);
	}
	quat QY(real speed)
	{
		real deg = speed;
		return quat(deg * PI / 180, vec3::UY);
	}
	quat QZ(real speed)
	{
		real deg = speed;
		return quat(deg * PI / 180, vec3::UZ);
	}
}