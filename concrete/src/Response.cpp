#include "Response.hpp"
#include "handleton.hpp"
#include "logger.hpp"

using namespace ilrd;

void Response::OnSuccess()
{
    Handleton::GetInstance<Logger>()->Log("Task succeeded", Logger::DEBUG);
}

void Response::OnFailure()
{
    Handleton::GetInstance<Logger>()->Log("Task failed", Logger::WARNING);
}
