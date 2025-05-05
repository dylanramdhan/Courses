from flask import Blueprint

grades_blueprint = Blueprint("grades", __name__)

@grades_blueprint.route("/grades/test")
def test_grades():
    return {"message": "Grades endpoint is working"}