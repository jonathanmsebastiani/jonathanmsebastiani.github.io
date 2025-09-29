const mongoose = require("mongoose");

const ScoreSchema = new mongoose.Schema({
    name: { type: String, required: true },
    score: { type: Number, required: true },
    date: { type: Date, default: Date.now } // Make sure this is a Date
});

module.exports = mongoose.model("Score", ScoreSchema);
