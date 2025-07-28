#include <Novice.h>
#include <cmath>
#include <imgui.h>

const char kWindowTitle[] = "LE2D_02_アオヤギ_ガクト_確認課題";

//----------------------------------------
// 構造体 
//----------------------------------------

struct Vector3 {
    float x, y, z;
    Vector3 operator+(const Vector3& rhs) const { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
    Vector3 operator-(const Vector3& rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
    Vector3 operator*(float s) const { return { x * s, y * s, z * s }; }
    Vector3 operator/(float s) const { return { x / s, y / s, z / s }; }
};

struct Matrix4x4 {
    float m[4][4];
    static Matrix4x4 Identity()
    {
        Matrix4x4 r {};
        r.m[0][0] = r.m[1][1] = r.m[2][2] = r.m[3][3] = 1.0f;
        return r;
    }
    Matrix4x4 operator*(const Matrix4x4& rhs) const
    {
        Matrix4x4 result {};
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    result.m[i][j] += m[i][k] * rhs.m[k][j];
                }
            }
        }
        return result;
    }
};

//----------------------------------------
// 行列関連
//----------------------------------------

Matrix4x4 MakeRotateX(float rad)
{
    Matrix4x4 m = Matrix4x4::Identity();
    float c = cosf(rad), s = sinf(rad);
    m.m[1][1] = c;
    m.m[1][2] = -s;
    m.m[2][1] = s;
    m.m[2][2] = c;
    return m;
}
Matrix4x4 MakeRotateY(float rad)
{
    Matrix4x4 m = Matrix4x4::Identity();
    float c = cosf(rad), s = sinf(rad);
    m.m[0][0] = c;
    m.m[0][2] = s;
    m.m[2][0] = -s;
    m.m[2][2] = c;
    return m;
}
Matrix4x4 MakeRotateZ(float rad)
{
    Matrix4x4 m = Matrix4x4::Identity();
    float c = cosf(rad), s = sinf(rad);
    m.m[0][0] = c;
    m.m[0][1] = -s;
    m.m[1][0] = s;
    m.m[1][1] = c;
    return m;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // キー入力結果を受け取る箱
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    // 初期ベクトル
    Vector3 a { 0.2f, 1.0f, 0.0f };
    Vector3 b { 2.4f, 3.1f, 1.2f };

    float angleX = 0.4f, angleY = 1.43f, angleZ = -0.8f;

    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {
        // フレームの開始
        Novice::BeginFrame();

        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        ///
        /// ↓更新処理ここから
        ///

        // ベクトル計算
        Vector3 c = a + b;
        Vector3 d = a - b;
        Vector3 e = a * 2.4f;

        // 回転行列の積
        Matrix4x4 rx = MakeRotateX(angleX);
        Matrix4x4 ry = MakeRotateY(angleY);
        Matrix4x4 rz = MakeRotateZ(angleZ);
        Matrix4x4 rotateMatrix = rx * ry * rz;

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        // GUI表示
        ImGui::Begin("Window");
        ImGui::DragFloat("angleX", &angleX, 0.01f);
        ImGui::DragFloat("angleY", &angleY, 0.01f);
        ImGui::DragFloat("angleZ", &angleZ, 0.01f);

        ImGui::Text("c = %.2f, %.2f, %.2f", c.x, c.y, c.z);
        ImGui::Text("d = %.2f, %.2f, %.2f", d.x, d.y, d.z);
        ImGui::Text("e = %.2f, %.2f, %.2f", e.x, e.y, e.z);

        ImGui::Separator();
        ImGui::Text("matrix:");
        for (int i = 0; i < 4; i++) {
            ImGui::Text("%.3f, %.3f, %.3f, %.3f",
                rotateMatrix.m[i][0], rotateMatrix.m[i][1],
                rotateMatrix.m[i][2], rotateMatrix.m[i][3]);
        }

        ImGui::End();

        ///
        /// ↑描画処理ここまで
        ///

        // フレームの終了
        Novice::EndFrame();

        // ESCキーが押されたらループを抜ける
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
            break;
        }
    }

    // ライブラリの終了
    Novice::Finalize();
    return 0;
}
