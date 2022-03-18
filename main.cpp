//
// Example that demonstrates offboard control using attitude, velocity control
// in NED (North-East-Down), and velocity control in body (Forward-Right-Down)
// coordinates.
//

#include <chrono>
#include <cmath>
#include <future>
#include <iostream>
#include <thread>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/offboard/offboard.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

using namespace mavsdk;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::this_thread::sleep_for;

void usage(const std::string& bin_name)
{
    std::cerr << "Usage : " << bin_name << " <connection_url>\n"
              << "Connection URL format should be :\n"
              << " For TCP : tcp://[server_host][:server_port]\n"
              << " For UDP : udp://[bind_host][:bind_port]\n"
              << " For Serial : serial:///path/to/serial/dev[:baudrate]\n"
              << "For example, to connect to the simulator use URL: udp://:14540\n";
}

std::shared_ptr<System> get_system(Mavsdk& mavsdk)
{
    std::cout << "Waiting to discover system...\n";
    auto prom = std::promise<std::shared_ptr<System>>{};
    auto fut = prom.get_future();

    // We wait for new systems to be discovered, once we find one that has an
    // autopilot, we decide to use it.
    mavsdk.subscribe_on_new_system([&mavsdk, &prom]() {
        auto system = mavsdk.systems().back();

        if (system->has_autopilot()) {
            std::cout << "Discovered autopilot\n";

            // Unsubscribe again as we only want to find one system.
            mavsdk.subscribe_on_new_system(nullptr);
            prom.set_value(system);
        }
    });

    // We usually receive heartbeats at 1Hz, therefore we should find a
    // system after around 3 seconds max, surely.
    if (fut.wait_for(seconds(3)) == std::future_status::timeout) {
        std::cerr << "No autopilot found.\n";
        return {};
    }

    // Get discovered system now.
    return fut.get();
}
void reset_velocity(int t,mavsdk::Offboard& offboard)
{
    Offboard::VelocityBodyYawspeed reset{};
    reset.yawspeed_deg_s = 0.0f;
    reset.down_m_s = 0.0f;
    reset.forward_m_s = 0.0f;
    reset.right_m_s = 0.0f;
    offboard.set_velocity_body(reset);
    sleep_for(seconds(t));

}
double degree_to_radian(double degree){

    double radian = 0.0;

    return  radian = (degree*(M_PI / 180));


}

bool offb_ctrl_body(mavsdk::Offboard& offboard)
{
    std::cout << "Starting Offboard velocity control in body coordinates\n";

    // Send it once before starting offboard, otherwise it will be rejected.
    Offboard::VelocityBodyYawspeed stay{};
    offboard.set_velocity_body(stay);

    Offboard::Result offboard_result = offboard.start();
    if (offboard_result != Offboard::Result::Success) {
        std::cerr << "Offboard start failed: " << offboard_result << '\n';
        return false;
    }
    std::cout << "Offboard started\n";

    std::cout << "North'a donuyorum \n";

    Offboard::VelocityBodyYawspeed turning_north_sitl{};
    turning_north_sitl.yawspeed_deg_s = -31.0f;
    turning_north_sitl.down_m_s = -0.66f;
    offboard.set_velocity_body(turning_north_sitl);
    sleep_for(seconds(3));
    reset_velocity(2,offboard);


    /*
    for(int i; i<250; i++){

        Offboard::VelocityBodyYawspeed cube_edge_1{};  //1
        cube_edge_1.forward_m_s = 2.5f;
        offboard.set_velocity_body(cube_edge_1);
        sleep_for(milliseconds(10));
        mavsdk::Telemetry::VelocityBody cube_edge_velocity;
        std::cout << " Velocity is : " << cube_edge_velocity << std::endl;



    } */
    float cube_vector_magnitude = 2.5f;
    int cube_edge_time = 2;

    Offboard::VelocityBodyYawspeed cube_edge_1{}; //1
    cube_edge_1.forward_m_s = cube_vector_magnitude;
    offboard.set_velocity_body(cube_edge_1);
    sleep_for(seconds(cube_edge_time));
    reset_velocity(1,offboard);
    std::cout << "Edge 1 bitti\n";


    Offboard::VelocityBodyYawspeed cube_edge_2{}; //2
    cube_edge_2.right_m_s = cube_vector_magnitude;
    offboard.set_velocity_body(cube_edge_2);
    sleep_for(seconds(cube_edge_time));
    reset_velocity(1,offboard);
    std::cout << "Edge 2 bitti\n";

    Offboard::VelocityBodyYawspeed cube_edge_3{};  //3
    cube_edge_3.forward_m_s = - cube_vector_magnitude;
    offboard.set_velocity_body(cube_edge_3);
    sleep_for(seconds(cube_edge_time));
    reset_velocity(1,offboard);
    std::cout << "Edge 3 bitti\n";

    Offboard::VelocityBodyYawspeed cube_edge_4{};  //4
    cube_edge_4.right_m_s = -cube_vector_magnitude;
    offboard.set_velocity_body(cube_edge_4);
    sleep_for(seconds(cube_edge_time));
    reset_velocity(1,offboard);
    std::cout << "Edge 4 bitti\n";

    Offboard::VelocityBodyYawspeed cube_edge_5{};  //5
    cube_edge_5.down_m_s = -cube_vector_magnitude;
    offboard.set_velocity_body(cube_edge_5);
    sleep_for(seconds(cube_edge_time));
    reset_velocity(1,offboard);
    std::cout << "Edge 5 bitti yukari ciktim\n";

    Offboard::VelocityBodyYawspeed cube_edge_6{};  //6
    cube_edge_6.forward_m_s = cube_vector_magnitude;
    offboard.set_velocity_body(cube_edge_6);
    sleep_for(seconds(cube_edge_time));
    reset_velocity(1,offboard);
    std::cout << "Edge 6 bitti\n";

    Offboard::VelocityBodyYawspeed cube_edge_7{};  //7
    cube_edge_7.right_m_s = cube_vector_magnitude;
    offboard.set_velocity_body(cube_edge_7);
    sleep_for(seconds(cube_edge_time));
    reset_velocity(1,offboard);
    std::cout << "Edge 7 bitti\n";

    Offboard::VelocityBodyYawspeed cube_edge_8{};  //8
    cube_edge_8.forward_m_s = -cube_vector_magnitude;
    offboard.set_velocity_body(cube_edge_8);
    sleep_for(seconds(cube_edge_time));
    reset_velocity(1,offboard);
    std::cout << "Edge 8 bitti\n";

    Offboard::VelocityBodyYawspeed cube_edge_9{};  //9
    cube_edge_9.right_m_s = -cube_vector_magnitude;
    offboard.set_velocity_body(cube_edge_9);
    sleep_for(seconds(cube_edge_time));
    reset_velocity(2,offboard);
    std::cout << "Edge 9 bitti\n";

    Offboard::VelocityBodyYawspeed cube_edge_10{};  //10
    cube_edge_10.right_m_s = cube_vector_magnitude;
    offboard.set_velocity_body(cube_edge_10);
    sleep_for(seconds(cube_edge_time));
    reset_velocity(1,offboard);
    std::cout << "Edge 10 bitti\n";

    Offboard::VelocityBodyYawspeed cube_edge_11{};  //11
    cube_edge_11.down_m_s = cube_vector_magnitude;
    offboard.set_velocity_body(cube_edge_11);
    sleep_for(seconds(cube_edge_time));
    reset_velocity(1,offboard);
    std::cout << "Edge 11 bitti\n";

    Offboard::VelocityBodyYawspeed cube_edge_12{};  //12
    cube_edge_12.forward_m_s = cube_vector_magnitude;
    offboard.set_velocity_body(cube_edge_12);
    sleep_for(seconds(cube_edge_time));
    reset_velocity(1,offboard);
    std::cout << "Edge 12 bitti\n";

    Offboard::VelocityBodyYawspeed cube_edge_13{};  //13
    cube_edge_13.down_m_s = -cube_vector_magnitude;
    offboard.set_velocity_body(cube_edge_13);
    sleep_for(seconds(cube_edge_time));
    reset_velocity(1,offboard);
    std::cout << "Edge 13 bitti\n";

    Offboard::VelocityBodyYawspeed cube_edge_14{};  //14
    cube_edge_14.right_m_s = -cube_vector_magnitude;
    offboard.set_velocity_body(cube_edge_14);
    sleep_for(seconds(cube_edge_time));
    reset_velocity(1,offboard);
    std::cout << "Edge 14 bitti\n";

    Offboard::VelocityBodyYawspeed cube_edge_15{};  //15
    cube_edge_15.down_m_s= cube_vector_magnitude;
    offboard.set_velocity_body(cube_edge_15);
    sleep_for(seconds(cube_edge_time));
    reset_velocity(1,offboard);
    std::cout << "Edge 15 bitti\n";

    Offboard::VelocityBodyYawspeed cube_edge_16{};  //16
    cube_edge_16.forward_m_s = -cube_vector_magnitude;
    offboard.set_velocity_body(cube_edge_16);
    sleep_for(seconds(cube_edge_time));
    reset_velocity(1,offboard);
    std::cout << "Edge 16 bitti\n";

    Offboard::VelocityBodyYawspeed after_cube_up{};  //triangle start position setup
    after_cube_up.down_m_s = -1.5f;
    offboard.set_velocity_body(after_cube_up);
    sleep_for(seconds(2));
    reset_velocity(1,offboard);
    std::cout << "Ucgen icin yukseliyorum \n";

    sleep_for(seconds (5));

    std::cout << "North'a donuyorum \n";

    /*
    Offboard::VelocityBodyYawspeed turning_north_sitl{};
    turning_north_sitl.yawspeed_deg_s = -31.0f;
    offboard.set_velocity_body(turning_north_sitl);
    sleep_for(seconds(3));
    reset_velocity(2,offboard);
    */

    std::cout << "Turning 60 degree\n";

    Offboard::VelocityBodyYawspeed turning_sixty{};
    turning_sixty.yawspeed_deg_s = 31.0f;
    offboard.set_velocity_body(turning_sixty);
    sleep_for(seconds(2));
    reset_velocity(1,offboard);
    std::cout << "Dondum\n";
    sleep_for(seconds(3));

    double velocity_vector_angle = 120.0;
    double yaw_change_rate = 0.0;
    double degisen_yaw = 0.0;
    double velocity_vector_magnitude = 0.00;
    double error = 1;
    for (int i=0; i<10;i++)
    {

        for(int j=0; j<3;j++)
        {
            velocity_vector_angle = 120;
            degisen_yaw=0;
            reset_velocity(1,offboard);
            double t = 0.00;
            auto start = std::chrono::high_resolution_clock::now();
            for(int k=0; k<500;k++)
            {

                velocity_vector_magnitude = sqrt(pow(sin(degree_to_radian(velocity_vector_angle)),2)+ pow(cos(
                        degree_to_radian(velocity_vector_angle)),2));
                std::cout << "Velocity vector magnitude" << velocity_vector_magnitude << std::endl;

                yaw_change_rate = (-0.08024* pow(t*velocity_vector_magnitude,7) + 1.02228* pow(t*velocity_vector_magnitude,6) - 4.2732 * pow(t*velocity_vector_magnitude,5) + 5.5667 * pow(t*velocity_vector_magnitude,4) + 1.02768 * pow(t*velocity_vector_magnitude,3) + 0.41919 * pow(t*velocity_vector_magnitude,2) + 3.56804 * (t*velocity_vector_magnitude) + 11.12216)*velocity_vector_magnitude*error;

                velocity_vector_angle = velocity_vector_angle + (yaw_change_rate/100);

                std::cout << "velocity vector angle: " << velocity_vector_angle << "  " <<  velocity_vector_magnitude * sin(
                        degree_to_radian(velocity_vector_angle))  << "  " <<  velocity_vector_magnitude * cos(degree_to_radian(velocity_vector_angle)) << "  " <<  0 << "  " << "Yaw change rate " << yaw_change_rate << "  " << degisen_yaw << std::endl ;

                Offboard::VelocityBodyYawspeed edge_drawer{};
                edge_drawer.yawspeed_deg_s = yaw_change_rate;
                edge_drawer.right_m_s = velocity_vector_magnitude * cos(degree_to_radian(velocity_vector_angle));
                edge_drawer.forward_m_s = velocity_vector_magnitude * sin(degree_to_radian(velocity_vector_angle));
                offboard.set_velocity_body(edge_drawer);
                sleep_for(milliseconds (10));


                t = t + 0.01;

                degisen_yaw = degisen_yaw + (yaw_change_rate*0.01);

            }
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << "Time taken by function: "
                 << duration.count() << " microseconds" << std::endl;

            error = error - 0.003;


        }



    }

    reset_velocity(1,offboard);

    sleep_for(seconds(100));

    /*
    Offboard::VelocityBodyYawspeed cc_and_climb{};
    cc_and_climb.down_m_s = -1.0f;
    cc_and_climb.yawspeed_deg_s = 60.0f;
    offboard.set_velocity_body(cc_and_climb);
    sleep_for(seconds(5));

    std::cout << "Turn back anti-clockwise\n";
    Offboard::VelocityBodyYawspeed ccw{};
    ccw.down_m_s = -1.0f;
    ccw.yawspeed_deg_s = -60.0f;
    offboard.set_velocity_body(ccw);
    sleep_for(seconds(5));

    std::cout << "Wait for a bit\n";
    offboard.set_velocity_body(stay);
    sleep_for(seconds(2));

    std::cout << "Fly a circle\n";
    Offboard::VelocityBodyYawspeed circle{};
    circle.forward_m_s = 5.0f;
    circle.yawspeed_deg_s = 30.0f;
    offboard.set_velocity_body(circle);
    sleep_for(seconds(15));

    std::cout << "Wait for a bit\n";
    offboard.set_velocity_body(stay);
    sleep_for(seconds(5));

    std::cout << "Fly a circle sideways\n";
    circle.right_m_s = -5.0f;
    circle.yawspeed_deg_s = 30.0f;
    offboard.set_velocity_body(circle);
    sleep_for(seconds(15));

    std::cout << "Wait for a bit\n";
    offboard.set_velocity_body(stay);
    sleep_for(seconds(8));
    */
    offboard_result = offboard.stop();
    if (offboard_result != Offboard::Result::Success) {
        std::cerr << "Offboard stop failed: " << offboard_result << '\n';
        return false;
    }
    std::cout << "Offboard stopped\n";

    return true;
}


int main(int argc, char** argv)
{

    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }

    Mavsdk mavsdk;
    ConnectionResult connection_result = mavsdk.add_any_connection(argv[1]);

    if (connection_result != ConnectionResult::Success) {
        std::cerr << "Connection failed: " << connection_result << '\n';
        return 1;
    }

    auto system = get_system(mavsdk);
    if (!system) {
        return 1;
    }
    std::cout << argv[1] << std::endl;
    // Instantiate plugins.
    auto action = Action{system};
    auto offboard = Offboard{system};
    auto telemetry = Telemetry{system};

    while (!telemetry.health_all_ok()) {
        std::cout << "Waiting for system to be ready\n";
        sleep_for(seconds(1));
    }
    std::cout << "System is ready\n";



    const auto arm_result = action.arm();
    if (arm_result != Action::Result::Success) {
        std::cerr << "Arming failed: " << arm_result << '\n';
        return 1;
    }
    std::cout << "Armed\n";

    const auto takeoff_result = action.takeoff();
    if (takeoff_result != Action::Result::Success) {
        std::cerr << "Takeoff failed: " << takeoff_result << '\n';
        return 1;
    }

    auto in_air_promise = std::promise<void>{};
    auto in_air_future = in_air_promise.get_future();
    telemetry.subscribe_landed_state([&telemetry, &in_air_promise](Telemetry::LandedState state) {
        if (state == Telemetry::LandedState::InAir) {
            std::cout << "Taking off has finished\n.";
            telemetry.subscribe_landed_state(nullptr);
            in_air_promise.set_value();
        }
    });
    in_air_future.wait_for(seconds(15));
    if (in_air_future.wait_for(seconds(3)) == std::future_status::timeout) {
        std::cerr << "Takeoff timed out.\n";
        return 1;
    }


    //  using body co-ordinates mission function
    if (!offb_ctrl_body(offboard)) {
        return 1;
    }



    const auto land_result = action.land();
    if (land_result != Action::Result::Success) {
        std::cerr << "Landing failed: " << land_result << '\n';
        return 1;
    }

    // Check if vehicle is still in air
    while (telemetry.in_air()) {
        std::cout << "Vehicle is landing...\n";
        sleep_for(seconds(1));
    }
    std::cout << "Landed!\n";

    // We are relying on auto-disarming but let's keep watching the telemetry for
    // a bit longer.
    sleep_for(seconds(3));
    std::cout << "Finished...\n";

    return 0;
}