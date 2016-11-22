# coding: utf-8
from peewee import *

# ������������ ����������� SQLite ���� ������ � ����������
db = SqliteDatabase('places.db')

# �����, ����������� ��������� ������ � ��: identifier - ����� ������������ �����, status - ��� ������
class Place(Model):
    identifier = IntegerField(unique=True)
    status = IntegerField()

    class Meta:
        database = db

# �����, �������������� ���������� ������������ ����� � ���� ������
def update_status(identifier, status):
    if status == STATUS_FREE or status == STATUS_RESERVED:
        if  1 << identifier << CAR_PLACE_COUNT:
            query = Place.update(status=status).where(Place.identifier == identifier)
            query.execute()
        else:
            return False
    else:
        return False

    return True

def get_status(identifier):
	if 1 << identifier << CAR_PLACE_COUNT:
		query = Place.get(Place.identifier == identifier)
        	query.execute()
		return query.status
        else:
            return None