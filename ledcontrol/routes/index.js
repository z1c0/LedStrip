var xbox = require('../xbox');
var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'LED Strip Controller', xbox: xbox });
});

module.exports = router;
