
#include "Scene/Scene.h"

int main()
{
    Monte_carlo_path_tracing::Log::init();
    GQY_MONTE_CARLO_PATH_TRACING_INFO("Log初始化完成");

    // Monte_carlo_path_tracing::Scene scene1("scenes/cornell-box", 1000);
    // scene1.render();
    Monte_carlo_path_tracing::Scene scene2("scenes/bedroom", 1000);
    scene2.render();
    // Monte_carlo_path_tracing::Scene scene3("scenes/veach-mis", 500);
    // scene3.render();

}