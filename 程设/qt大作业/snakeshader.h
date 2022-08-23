#ifndef SNAKESHADER_H
#define SNAKESHADER_H
#include <QString>
#include <QOpenGLShaderProgram>

class SnakeShader
{
public:
    QOpenGLShaderProgram shader;

    SnakeShader(const QString& vertex_shader_path, const QString& fragment_shader_path);
    ~SnakeShader();
    void use();
    void setUniformBool(const QString& name, const bool& value);
    void setUniformInt(const QString& name, const int& value);
    void setUniformFloat(const QString& name, const float& value);
    void setUniformVec2(const QString& name, const QVector2D& value);
    void setUniformVec3(const QString& name, const QVector3D& value);
    void setUniformVec4(const QString& name, const QVector4D& value);
    void setUniformMat2(const QString& name, const QMatrix2x2& value);
    void setUniformMat3(const QString& name, const QMatrix3x3& value);
    void setUniformMat4(const QString& name, const QMatrix4x4& value);

};

#endif // SNAKESHADER_H
