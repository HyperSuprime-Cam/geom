/*
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "pybind11/pybind11.h"
#include "pybind11/eigen.h"
#include "pybind11/stl.h"

#include "ndarray/pybind11.h"

#include "lsst/utils/python.h"

#include "lsst/geom/Extent.h"
#include "lsst/geom/Point.h"
#include "lsst/geom/LinearTransform.h"

namespace py = pybind11;
using namespace pybind11::literals;

namespace lsst {
namespace geom {
namespace {

using PyLinearTransform = py::class_<LinearTransform, std::shared_ptr<LinearTransform>>;

PYBIND11_MODULE(linearTransform, mod) {
    py::module::import("lsst.geom.coordinates");

    PyLinearTransform cls(mod, "LinearTransform");

    // Parameters enum is really only used as integer constants.
    cls.attr("XX") = py::cast(int(LinearTransform::Parameters::XX));
    cls.attr("YX") = py::cast(int(LinearTransform::Parameters::YX));
    cls.attr("XY") = py::cast(int(LinearTransform::Parameters::XY));
    cls.attr("YY") = py::cast(int(LinearTransform::Parameters::YY));

    /* Constructors */
    cls.def(py::init<>());
    cls.def(py::init<LinearTransform::Matrix const &>(), "matrix"_a);

    /* Operators */
    cls.def("__call__", (Point2D(LinearTransform::*)(Point2D const &) const) & LinearTransform::operator());
    cls.def("__call__", (Extent2D(LinearTransform::*)(Extent2D const &) const) & LinearTransform::operator());
    cls.def("__getitem__",
            [](LinearTransform const &self, int i) { return self[lsst::utils::python::cppIndex(4, i)]; });
    cls.def("__getitem__", [](LinearTransform const &self, std::pair<int, int> i) {
        auto row = lsst::utils::python::cppIndex(2, i.first);
        auto col = lsst::utils::python::cppIndex(2, i.second);
        return self.getMatrix()(row, col);
    });
    cls.def("__mul__", &LinearTransform::operator*, py::is_operator());
    cls.def("__add__", &LinearTransform::operator+, py::is_operator());
    cls.def("__sub__", &LinearTransform::operator-, py::is_operator());
    cls.def("__iadd__", &LinearTransform::operator+=);
    cls.def("__isub__", &LinearTransform::operator-=);

    /* Members */
    cls.def_static("makeScaling", (LinearTransform(*)(double))LinearTransform::makeScaling, "scale"_a);
    cls.def_static("makeScaling", (LinearTransform(*)(double, double))LinearTransform::makeScaling);
    cls.def_static("makeRotation", (LinearTransform(*)(Angle t))LinearTransform::makeRotation, "angle"_a);
    cls.def("getParameterVector", &LinearTransform::getParameterVector);
    cls.def("getMatrix",
            (LinearTransform::Matrix const &(LinearTransform::*)() const) & LinearTransform::getMatrix);
    cls.def("invert", &LinearTransform::invert);
    cls.def("computeDeterminant", &LinearTransform::computeDeterminant);
    cls.def("isIdentity", &LinearTransform::isIdentity);

    cls.def("set",
            [](LinearTransform &self, double xx, double yx, double xy, double yy) {
                self[LinearTransform::XX] = xx;
                self[LinearTransform::XY] = xy;
                self[LinearTransform::YX] = yx;
                self[LinearTransform::YY] = yy;
            },
            "xx"_a, "yx"_a, "xy"_a, "yy"_a);

    cls.def("__str__", [](LinearTransform const &self) { return py::str(py::cast(self.getMatrix())); });
    cls.def("__repr__", [](LinearTransform const &self) {
        return py::str("LinearTransform(\n{}\n)").format(py::cast(self.getMatrix()));
    });
    cls.def("__reduce__", [cls](LinearTransform const &self) {
        return py::make_tuple(cls, py::make_tuple(py::cast(self.getMatrix())));
    });
}

}  // namespace
}  // namespace geom
}  // namespace lsst
