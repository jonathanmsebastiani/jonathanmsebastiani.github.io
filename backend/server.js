require("dotenv").config();
const express = require("express");
const mongoose = require("mongoose");
const cors = require("cors");

const Score = require("./models/Score");

const app = express();
const PORT = process.env.PORT || 4000;

// Middleware
app.use(cors());
app.use(express.json());

if (!process.env.MONGODB_URI) {
  console.error("MONGODB_URI is not defined!");
  process.exit(1); // stop the server
}

// Connect to MongoDB
mongoose
  .connect(process.env.MONGODB_URI)
  .then(() => console.log("MongoDB connected"))
  .catch((err) => console.error(err));

// Routes

app.get("/scores", async (req, res) => {
  console.log("GET /scores was hit");
  try {
    const scores = await Score.find();
    res.json(scores);
  } catch (err) {
    res.status(500).send("Server error: " + err.message);
  }
});

// Add a new score
app.post("/scores", async (req, res) => {
  try {
    const { name, score } = req.body;
    if (!name || !score) {
      return res.status(400).json({ error: "Name and score are required" });
    }

    const newScore = new Score({ name, score });
    await newScore.save();
    res.status(201).json(newScore);
  } catch (error) {
    console.error(error);
    res.status(500).json({ error: "Server error" });
  }
});

app.get("/scores", async (req, res) => {
  try {
    const scores = await Score.find(); // mongoose model
    res.json(scores);
  } catch (err) {
    res.status(500).send("Server error: " + err.message);
  }
});

app.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});