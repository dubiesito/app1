import 'dart:developer';

import 'package:flutter/material.dart';
import '../model/counter_model.dart';

class CounterViewmodel extends ChangeNotifier{
    //save actual status
    CounterModel model = CounterModel(0);
    //exponcer los datos a la vista
    int get count => model.count;
    //crear funciones de incrementar y decrementar
}