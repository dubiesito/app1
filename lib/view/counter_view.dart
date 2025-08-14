import 'package:flutter/material.dart';

class CounterView extends StatelessWidget {
  const CounterView({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Contador con MVM'),
      ),
      body: const Center(
        child: Text("Hola mundo"),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: () {
          // no hace nada
        },
        child: const Icon(Icons.add), // icono
      ),
    );
  }
}
