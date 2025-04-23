setwd("~/Documents/Network-Analysis/Final Project")

library(igraph)
library(tidyverse)

# Research Question:
# Which airports have the highest variation of flight destinations in the US during 2010? How do the airports associated
# with this massive amount of traffic flow deal with these unusually high amounts of passengers?


# Read the edges data (assuming it is in a file named 'edges.txt')
airport_links <- read.table("http://opsahl.co.uk/tnet/datasets/USairport500.txt", header = F, as.is = T)

# Read airport names
airport_nodes <- read.table("http://opsahl.co.uk/tnet/datasets/USairport_2010_codes.txt", header = F, as.is = T)

# Links only have top 500 airports so I will reduce the nodes data frame
airport_nodes <- head(airport_nodes, 500)

## Now create the graph object
Airport_Network <- graph_from_data_frame(d=airport_links, vertices=airport_nodes, directed = TRUE)

# Assigning names to the airport network
V(Airport_Network)$name <- airport_nodes$V2
E(Airport_Network)$weights <- airport_links$V3

# Calculate degree centrality for each node (in and out)
node_degree <- degree(Airport_Network, mode="all")

set.seed(2022)
plot(Airport_Network, 
     vertex.size= node_degree / 15, 
     vertex.color="orange",
     edge.arrow.size= 0,
     vertex.label = airport_nodes$V2, 
     vertex.label.cex = 0.5,
     vertex.label.color = "blue",
     vertex.label.font=2
     )

# Lets try and cluster this graph based on edge betweenness.
# Edge betweenness quantifies the number of shortest paths between 
# pairs of nodes that pass through the edge. Edges with high betweenness 
# are considered crucial for maintaining the connectivity within the network 
# because they act as bridges over which many shortest paths traverse.

Airport_Edge_Betweenness <- cluster_edge_betweenness(Airport_Network, modularity = TRUE)

# Plot
set.seed(2022)
plot(Airport_Edge_Betweenness,
     Airport_Network, 
     vertex.size= node_degree / 20, 
     vertex.color="orange",
     edge.arrow.size= 0,
     vertex.label = NA, 
     vertex.label.cex = 0.5,
     vertex.label.color = "blue",
     edge.color = "darkgray",
     edge.width = 0.5)

# There is a lot of information here, so lets reduce the data to the top 5% of airports
# with the most 

# Find the top 5% of nodes by degree (strength)
node_strengths <- strength(Airport_Network, mode="all", weights=E(Airport_Network)$weight)
top_5_percent_threshold <- quantile(node_strengths, 0.95)
top_5_percent_nodes <- V(Airport_Network)[node_strengths >= top_5_percent_threshold]

# Subset the graph to include only the top 5% nodes
Top_5_Percent_Network <- induced_subgraph(Airport_Network, top_5_percent_nodes)

# Plot the top 5 percent graph
set.seed(2022)
plot(Top_5_Percent_Network,
     vertex.size=node_strengths[top_5_percent_nodes] / 20,
     vertex.color="orange",
     edge.arrow.size=0,
     vertex.label=V(Top_5_Percent_Network)$name,
     vertex.label.cex=0.7,
     vertex.label.color="blue",
     vertex.label.font=2)

# Lets see how Edge Betweenness acts now that 
# we've reduced the data to the top 5 percent.

Top_5_Percent_Network_Edge_Betweenness <- cluster_edge_betweenness(Top_5_Percent_Network, modularity = TRUE)

# Plot the top 5 percent graph with edge betweeness
set.seed(2022)
plot(Top_5_Percent_Network_Edge_Betweenness,
     Top_5_Percent_Network,
     vertex.size=node_strengths[top_5_percent_nodes] / 20,
     vertex.color="orange",
     edge.arrow.size=0,
     vertex.label=V(Top_5_Percent_Network)$name,
     vertex.label.cex=0.7,
     vertex.label.color="blue",
     vertex.label.font=2)

# What is the edge density of the Top 5% Airport Network?
edge_density(Top_5_Percent_Network)
# 0.9566667

# What is the compactness of the Top 5% Airport Network?
compactness <- function(Top_5_Percent_Network) {
  gra.geo <- distances(Top_5_Percent_Network) ## generate geodesic distances
  gra.rdist <- 1/gra.geo  ## reciprocal of geodesics
  diag(gra.rdist) <- NA   ## assign NA to diagonal
  gra.rdist[gra.rdist == Inf] <- 0 ## replace infinity with 0
  # Compactness = mean of reciprocal distances
  comp.igph <- mean(gra.rdist, na.rm=TRUE) 
  return(comp.igph)
}

compactness(Top_5_Percent_Network)
# 0.9783333

# What is the average degree of a node?
mean(degree(Top_5_Percent_Network, mode = "out"))
# 22.96 out of 24 other nodes total.
# On average, an airport will have flights to 95.67% of the other
# 24 airports in the network.


# Conclusions:
# Airports that have a high degree centrallity, have a high connection of degree with
# other airports that also have a high degree centrallity. We can conclude that there is
# a high amount of 
