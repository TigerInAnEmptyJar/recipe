# coding: utf-8

from __future__ import absolute_import
from datetime import date, datetime  # noqa: F401

from typing import List, Dict  # noqa: F401

from swagger_server.models.base_model_ import Model
from swagger_server import util


class Shopping(Model):
    """NOTE: This class is auto generated by the swagger code generator program.

    Do not edit the class manually.
    """

    def __init__(self, title: str=None, id: str=None, days: List[object]=None):  # noqa: E501
        """Shopping - a model defined in Swagger

        :param title: The title of this Shopping.  # noqa: E501
        :type title: str
        :param id: The id of this Shopping.  # noqa: E501
        :type id: str
        :param days: The days of this Shopping.  # noqa: E501
        :type days: List[object]
        """
        self.swagger_types = {
            'title': str,
            'id': str,
            'days': List[object]
        }

        self.attribute_map = {
            'title': 'title',
            'id': 'id',
            'days': 'days'
        }

        self._title = title
        self._id = id
        self._days = days

    @classmethod
    def from_dict(cls, dikt) -> 'Shopping':
        """Returns the dict as a model

        :param dikt: A dict.
        :type: dict
        :return: The Shopping of this Shopping.  # noqa: E501
        :rtype: Shopping
        """
        return util.deserialize_model(dikt, cls)

    @property
    def title(self) -> str:
        """Gets the title of this Shopping.


        :return: The title of this Shopping.
        :rtype: str
        """
        return self._title

    @title.setter
    def title(self, title: str):
        """Sets the title of this Shopping.


        :param title: The title of this Shopping.
        :type title: str
        """

        self._title = title

    @property
    def id(self) -> str:
        """Gets the id of this Shopping.


        :return: The id of this Shopping.
        :rtype: str
        """
        return self._id

    @id.setter
    def id(self, id: str):
        """Sets the id of this Shopping.


        :param id: The id of this Shopping.
        :type id: str
        """

        self._id = id

    @property
    def days(self) -> List[object]:
        """Gets the days of this Shopping.


        :return: The days of this Shopping.
        :rtype: List[object]
        """
        return self._days

    @days.setter
    def days(self, days: List[object]):
        """Sets the days of this Shopping.


        :param days: The days of this Shopping.
        :type days: List[object]
        """

        self._days = days