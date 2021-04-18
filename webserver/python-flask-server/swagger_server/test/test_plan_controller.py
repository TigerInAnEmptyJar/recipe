# coding: utf-8

from __future__ import absolute_import

from flask import json
from six import BytesIO

from swagger_server.models.plan import Plan  # noqa: E501
from swagger_server.models.planlist import Planlist  # noqa: E501
from swagger_server.models.subscription import Subscription  # noqa: E501
from swagger_server.test import BaseTestCase


class TestPlanController(BaseTestCase):
    """PlanController integration test stubs"""

    def test_add_plan(self):
        """Test case for add_plan

        Add a plan
        """
        plan = Plan()
        response = self.client.open(
            '/v1/plan',
            method='POST',
            data=json.dumps(plan),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_change_plan(self):
        """Test case for change_plan

        Change an existing meal plan
        """
        plan = Plan()
        response = self.client.open(
            '/v1/plan/{planId}'.format(planId='planId_example'),
            method='PUT',
            data=json.dumps(plan),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_delete_plan(self):
        """Test case for delete_plan

        Deletes a meal plan
        """
        response = self.client.open(
            '/v1/plan/{planId}'.format(planId='planId_example'),
            method='DELETE')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_get_plan(self):
        """Test case for get_plan

        Retrieve a meal plan
        """
        response = self.client.open(
            '/v1/plan/{planId}'.format(planId='planId_example'),
            method='GET')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_get_plan_list(self):
        """Test case for get_plan_list

        Get list of available meal plans
        """
        response = self.client.open(
            '/v1/plan',
            method='GET')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_subscribe(self):
        """Test case for subscribe

        Subscribe to a day in a meal plan
        """
        subscriber = Subscription()
        response = self.client.open(
            '/v1/plan/subscribe/{planId}'.format(planId='planId_example'),
            method='PUT',
            data=json.dumps(subscriber),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_unsubscribe(self):
        """Test case for unsubscribe

        Unsubscribe from a day in a meal plan
        """
        subscriber = Subscription()
        response = self.client.open(
            '/v1/plan/unsubscribe/{planId}'.format(planId='planId_example'),
            method='PUT',
            data=json.dumps(subscriber),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))


if __name__ == '__main__':
    import unittest
    unittest.main()
