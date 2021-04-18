import connexion
import six

from swagger_server.models.plan import Plan  # noqa: E501
from swagger_server.models.planlist import Planlist  # noqa: E501
from swagger_server.models.subscription import Subscription  # noqa: E501
from swagger_server import util

allPlans = {}
aPlanlist = []

def add_plan(plan):  # noqa: E501
    """Add a plan

    Use this to add a meal plan to the list of available meal plans on the server # noqa: E501

    :param plan: Meal plan to add to the server
    :type plan: dict | bytes

    :rtype: None
    """
    if connexion.request.is_json:
        plan = Plan.from_dict(connexion.request.get_json())  # noqa: E501
    if not plan.id in allPlans:
        allPlans.update({plan.id: plan})
        aPlanlist.append({'id': plan.id, 'title': plan.name})
    else:
        return 'Plan exists already', 405
    return 'Successful operation', 200


def change_plan(planId, plan):  # noqa: E501
    """Change an existing meal plan

    Here we change a meal plan that already exists on the server. # noqa: E501

    :param planId: ID of meal plan to change
    :type planId:
    :param plan: The changed meal plan
    :type plan: dict | bytes

    :rtype: Plan
    """
    if not planId in allPlans:
        return 'Meal plan not found', 404
    if connexion.request.is_json:
        plan = Plan.from_dict(connexion.request.get_json())  # noqa: E501
        allPlans[planId] = plan
    return allPlans[planId], 200


def delete_plan(planId):  # noqa: E501
    """Deletes a meal plan

    Here a meal plan is deleted from the server # noqa: E501

    :param planId: Id of meal plan to delete
    :type planId:

    :rtype: Planlist
    """
    if not plan.id in allPlans:
        return 'Meal plan not found', 404
    del aPlanlist[aPlanlist.index({'id': planId, 'title': aPlanlist[planId].name})]
    del allPlans[planId]
    return aPlanlist, 200


def get_plan(planId):  # noqa: E501
    """Retrieve a meal plan

    Retrieve detailed information about a single meal plan. # noqa: E501

    :param planId: ID of meal plan to return
    :type planId:

    :rtype: Plan
    """
    if not planId in allPlans:
        return 'Meal plan not found', 404
    return allPlans[planId], 200


def get_plan_list():  # noqa: E501
    """Get list of available meal plans

    Here we return a list of all available meal plans on the server. The list itself only contains ID and name of the plan. Use the ID to get all details about the plan. # noqa: E501


    :rtype: Planlist
    """
    return aPlanlist, 200


def subscribe(planId, subscriber):  # noqa: E501
    """Subscribe to a day in a meal plan

    Subscribe # noqa: E501

    :param planId: Id of the meal plan to subscribe to
    :type planId:
    :param subscriber: The name of the person subscribing to the meal
    :type subscriber: dict | bytes

    :rtype: Plan
    """

    if not planId in allPlans:
        return 'Meal plan not found', 404
    if connexion.request.is_json:
        subs = Subscription.from_dict(connexion.request.get_json())  # noqa: E501
        dayId = subs.day
    if len(allPlans[planId].items) <= dayId:
        return 'Day not found', 404
    if subs.subscriber not in allPlans[planId].items[dayId]["subscribers"]:
        allPlans[planId].items[dayId]["subscribers"].append(subs.subscriber)
        return allPlans[planId], 200
    return 'Subscriber already added', 400


def unsubscribe(planId, subscriber):  # noqa: E501
    """Unsubscribe from a day in a meal plan

    Unsubscribe # noqa: E501

    :param planId: Id of the meal plan to unsubscribe from
    :type planId:
    :param subscriber: The name of the person unsubscribing from the meal
    :type subscriber: dict | bytes

    :rtype: Plan
    """
    if not planId in allPlans:
        return 'Meal plan not found', 404
    if connexion.request.is_json:
        subs = Subscription.from_dict(connexion.request.get_json())  # noqa: E501
        dayId = subs.day
    if len(allPlans[planId].items) <= dayId:
        return 'Day not found', 404
    if subscriber not in allPlans[planId].items[dayId]["subscribers"]:
        allPlans[planId].items[dayId]["subscribers"].remove(subs.subscriber)
        return allPlans[planId], 200
    return 'Subscriber not found', 400
