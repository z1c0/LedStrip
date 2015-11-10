var xbox = require('../xbox');
var express = require('express');
var router = express.Router();

var title = "LED Strip Controller";

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: title});
});

/* GET home page. */
router.get('/diag', function(req, res, next) {
  res.render('index', { title: title, xbox: xbox });
});

module.exports = router;
