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

/*
 * Example for usage of AffineTransform and LinearTransform classes
 */

#include <iostream>

#include "Eigen/Core"

#include "lsst/geom.h"

using namespace std;

void linearTransformExample() {
    // Default Construction
    lsst::geom::LinearTransform def;

    // inspect the transform
    cout << "default LinearTransform matrix: " << def.getMatrix() << endl;
    cout << "default LinearTransform parameters: " << def.getParameterVector() << endl;

    // Copy construct a scaling transform
    lsst::geom::LinearTransform s = lsst::geom::LinearTransform::makeScaling(1.5);
    cout << "scaling LinearTransform matrix: " << s.getMatrix() << endl;
    cout << "scaling LinearTransform parameters: " << s.getParameterVector() << endl;

    // copy construt a rotation transform
    lsst::geom::LinearTransform r = lsst::geom::LinearTransform::makeRotation(1.0 * lsst::geom::radians);
    cout << "rotation LinearTransform matrix: " << r.getMatrix() << endl;
    cout << "rotation LinearTransform parameters: " << r.getParameterVector() << endl;

    // concactenate the scaling and rotation transform
    lsst::geom::LinearTransform c = s * r;
    cout << "rotation+scaling LinearTransform matrix: " << c.getMatrix() << endl;
    cout << "rotation+scaling LinearTransform parameters: " << c.getParameterVector() << endl;

    // create a point, and duplicate it as an extent
    lsst::geom::Point2D point = lsst::geom::Point2D(3.0, 4.5);
    lsst::geom::Extent2D extent(point);

    // apply the LinearTransforms to points and extents. Because there is no
    // translataion component to a LinearTransform, this operation is equivalent
    // on points and extents
    cout << "original point: " << point << "\tTransformed point: " << c(point) << endl;
    cout << "original extent: " << extent << "\tTransformed extent: " << c(extent) << endl;

    // The Affine transform can also compute the derivative of the transformation
    // with respect to the transform parameters
    cout << "Transformation derivative: " << c.dTransform(point) << endl;
}

void affineTransformExample() {
    // Default Construction
    lsst::geom::AffineTransform def;

    // inspect the transform
    cout << "default AffineTransform matrix: " << def.getMatrix() << endl;
    cout << "default AffineTransform parameters: " << def.getParameterVector() << endl;

    // Copy construct a scaling transform
    lsst::geom::AffineTransform s = lsst::geom::AffineTransform::makeScaling(1.5);
    cout << "scaling AffineTransform matrix: " << s.getMatrix() << endl;
    cout << "scaling AffineTransform parameters: " << s.getParameterVector() << endl;

    // copy construt a rotation transform
    lsst::geom::AffineTransform r = lsst::geom::AffineTransform::makeRotation(1.0 * lsst::geom::radians);
    cout << "rotation AffineTransform matrix: " << r.getMatrix() << endl;
    cout << "rotation AffineTransform parameters: " << r.getParameterVector() << endl;

    // copy construct a translation transform
    lsst::geom::AffineTransform t =
            lsst::geom::AffineTransform::makeTranslation(lsst::geom::Extent2D(15.0, 10.3));
    cout << "translation AffineTransform matrix: " << t.getMatrix() << endl;
    cout << "translation AffineTransform parameters: " << t.getParameterVector() << endl;

    // concactenate the scaling and rotation transform
    lsst::geom::AffineTransform c = s * r * t;
    cout << "translation+rotation+scaling AffineTransform matrix: " << c.getMatrix() << endl;
    cout << "translation+rotation+scaling AffineTransform parameters: " << c.getParameterVector() << endl;

    // We can grab just the Linear part of the AffineTransform
    cout << "linear part of affine: " << c.getLinear() << endl;

    // or we cna grab the translation
    cout << "translation part of affine: " << c.getTranslation() << endl;

    // create a point, and duplicate it as an extent
    lsst::geom::Point2D point = lsst::geom::Point2D(3.0, 4.5);
    lsst::geom::Extent2D extent(point);

    // apply the LinearTransforms to points and extents. Because there a
    // translataion component to a AffineTransform, this operation is not
    // equivalent on points and extents
    cout << "original point: " << point << "\tTransformed point: " << c(point) << endl;
    cout << "original extent: " << extent << "\tTransformed extent: " << c(extent) << endl;

    // The Affine transform can also compute the derivative of the transformation
    // with respect to the transform parameters
    cout << "point transformation derivative: " << c.dTransform(point) << endl;
    cout << "extent transformation derivative: " << c.dTransform(extent) << endl;
}

int main() {
    linearTransformExample();
    affineTransformExample();

    return 0;
}
