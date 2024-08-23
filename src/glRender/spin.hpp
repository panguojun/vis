/**					�����������Կ���һ����ת
*					ʹ����ת���Ǻ����Զ�ʵ��
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