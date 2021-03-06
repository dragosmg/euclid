% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/geometry_locations.R
\name{barycenter}
\alias{barycenter}
\title{Calculate barycenter of a set of weighted points}
\usage{
barycenter(x, y, z = NULL, t = NULL)
}
\arguments{
\item{x, y, z, t}{vectors of weighted points. \code{z} and \code{t} are
optional}
}
\value{
A point vector
}
\description{
The barycenter is the center of mass of 2 or more points. euclid supports
calculating the barycenter of up to 4 weighted points. If the weights are the
same between the points the barycenter corresponds to the centroid (or
midpoint in the case of two points).
}
\examples{
p <- weighted_point(
  x = c(4, 0, -3),
  y = c(2, -4, 7),
  w = c(20, 8, 13)
)
bc <- barycenter(p[1], p[2], p[3])
bc

plot(p, cex = as.numeric(parameter(p, "w") / 8))
euclid_plot(bc, pch = 16, col = "firebrick", cex = 3)

}
