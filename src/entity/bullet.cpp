#include "bullet.h"
#include <string>
#include <sstream>

std::string Bullet::get_bullet_type()
{
    std::stringstream info_stream;

    info_stream << "Bullet type: " << static_cast<unsigned>(Bullet::type) << std::endl;
    return info_stream.str();
}
