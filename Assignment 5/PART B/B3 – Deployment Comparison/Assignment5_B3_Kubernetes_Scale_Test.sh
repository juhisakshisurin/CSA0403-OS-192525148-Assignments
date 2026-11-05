#!/usr/bin/env bash
# B3.3 helper for a local kind/minikube cluster.
set -euo pipefail

echo "Apply deployment and service"
kubectl apply -f kubernetes/deployment.yaml

echo "Wait for three Pods"
kubectl rollout status deployment/result-portal --timeout=180s
kubectl get pods -l app=result-portal -o wide

echo "Delete one Pod; Deployment/ReplicaSet should recreate it"
POD="$(kubectl get pods -l app=result-portal -o jsonpath='{.items[0].metadata.name}')"
echo "Deleting: $POD"
kubectl delete pod "$POD"
kubectl get pods -l app=result-portal -w
